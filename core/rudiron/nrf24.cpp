#include "nrf24.h"

namespace Rudiron
{
    nRF24::nRF24()
    {
    }

    bool nRF24::begin(bool receiver)
    {
        nRF24_GPIO_Init();

        Rudiron::SPI::getSPI2().begin();

//работа через прерывания
#ifdef NRF24_USE_INTERRUPT
        nRF24_InitIRQ();
#endif

        // RX/TX disabled
        nRF24_CE_L;
        Rudiron::CLK::delay_millis(10);

        int repeat = 10;
        while (!nRF24_Check() && repeat > 0)
        {
            Rudiron::CLK::delay_millis(10);
            repeat--;
        }
        if (repeat <= 0)
        {
            end();
            return false;
        }

        // Initialize the nRF24L01 to its default state
        nRF24_Init();

        nRF24_SetRFChannel(40);

        // Set data rate
        nRF24_SetDataRate(nRF24_DR_250kbps);

        // Set CRC scheme
        nRF24_SetCRCScheme(nRF24_CRC_2byte);

        // Set address width, its common for all pipes (RX and TX)
        nRF24_SetAddrWidth(5);

        // Configure RX PIPE
        static const uint8_t nRF24_ADDR[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xF0};

        //Адрес трубы приемника
        nRF24_SetAddr(nRF24_PIPE0, nRF24_ADDR);

        if (receiver)
        {
            nRF24_SetRXPipe(nRF24_PIPE0, nRF24_AA_ON, NRF24_PAYLOAD_LENGTH);

            nRF24_SetTXPower(nRF24_TXPWR_18dBm);

            // Set operational mode (PRX == receiver)
            nRF24_SetOperationalMode(nRF24_MODE_RX);

            // Clear any pending IRQ flags
            nRF24_ClearIRQFlags();

            // Wake the transceiver
            nRF24_SetPowerMode(nRF24_PWR_UP);

            // Put the transceiver to the RX mode
            nRF24_CE_H;
        }
        else
        {
            //Адрес трубы передатчика
            nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR);

            // Set TX power (maximum)
            nRF24_SetTXPower(nRF24_TXPWR_0dBm);

            // Configure auto retransmit: 10 retransmissions with pause of 2500s in between
            nRF24_SetAutoRetr(nRF24_ARD_2500us, 10);

            // Enable Auto-ACK for pipe#0 (for ACK packets)
            nRF24_EnableAA(nRF24_PIPE0);

            /// Set operational mode (PTX == transmitter)
            nRF24_SetOperationalMode(nRF24_MODE_TX);

            // Clear any pending IRQ flags
            nRF24_ClearIRQFlags();

            // Wake the transceiver
            nRF24_SetPowerMode(nRF24_PWR_UP);
        }

        return true;
    }

    void nRF24::end()
    {
        Rudiron::SPI::getSPI2().end();
        nRF24_GPIO_DeInit();
        nRF24_DeInitIRQ();
    }

    int nRF24::available()
    {
#if NRF24_RX_BUFFER_LENGTH > 0
        bool endofstream = _nrf24_rx_buffer_tail == _nrf24_rx_buffer_head;

#ifndef NRF24_USE_INTERRUPT
        if (endofstream)
        {
            return nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY;
        }
#endif

        return !endofstream;
#else
        return 0;
#endif
    }

    int nRF24::peek(void)
    {
#if NRF24_RX_BUFFER_LENGTH > 0
        return _nrf24_rx_buffer[_nrf24_rx_buffer_tail];
#else
        return EndOfStream;
#endif
    }

    int nRF24::read(void)
    {
#ifndef NRF24_USE_INTERRUPT
        uint8_t nRF24_payload[NRF24_PAYLOAD_LENGTH];
        nRF24_RXResult pipe = nRF24_RX_EMPTY;

        // Length of received payload
        uint8_t payload_length;
        if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY)
        {
            // Get a payload from the transceiver
            pipe = nRF24_ReadPayload(nRF24_payload, &payload_length);
        }

        if (payload_length != NRF24_PAYLOAD_LENGTH)
        {
            //ошибка!
            return EndOfStream;
        }

        //Заполнение кольцевого буфера
        uint8_t package_length = nRF24_payload[0];

        for (uint8_t i = 1; i < package_length + 1; i++)
        {
            uint8_t data = nRF24_payload[i];

            NRF24_BUFFER_INDEX_T next_head = _nrf24_rx_buffer_head + 1;
            if (next_head == NRF24_RX_BUFFER_LENGTH)
            {
                next_head = 0;
            }

            if (next_head != _nrf24_rx_buffer_tail)
            {
                _nrf24_rx_buffer[_nrf24_rx_buffer_head] = data;
                _nrf24_rx_buffer_head = next_head;
            }
        }
#endif
        //Чтение кольцевого буфера
        if (_nrf24_rx_buffer_tail == _nrf24_rx_buffer_head)
        {
            //нет данных для чтения
            return EndOfStream;
        }

        int data = _nrf24_rx_buffer[_nrf24_rx_buffer_tail];
        _nrf24_rx_buffer_tail = (NRF24_BUFFER_INDEX_T)(_nrf24_rx_buffer_tail + 1) % NRF24_RX_BUFFER_LENGTH;
        return data;
    }

    int nRF24::availableForWrite()
    {
        return nRF24_GetStatus_TXFIFO() == nRF24_STATUS_TXFIFO_EMPTY;
    }

    void nRF24::flush()
    {
        nRF24_FlushTX();
        while (nRF24_GetStatus_TXFIFO() != nRF24_STATUS_TXFIFO_EMPTY)
        {
        }
    }

    size_t nRF24::write(uint8_t byte)
    {
        transmit(&byte, 1);
    }

    size_t nRF24::write(const uint8_t *buffer, size_t size)
    {
        const int integer_length = NRF24_PAYLOAD_LENGTH - 1;
        size_t sent_counter = 0;

        uint8_t dividend = size / integer_length;
        uint8_t remainder = size % integer_length;

        size_t buffer_offset = 0;

        if (dividend)
        {
            for (uint8_t i = 0; i < dividend; i++)
            {
                buffer_offset = i * integer_length;
                sent_counter += transmit(buffer + buffer_offset, integer_length);
            }
            buffer_offset += integer_length;
        }

        sent_counter += transmit(buffer + buffer_offset, remainder);
        return sent_counter;
    }

    size_t nRF24::transmit(const uint8_t *buffer, uint8_t length)
    {
        flush();
        // uint8_t packets_lost = 0;
        uint8_t nRF24_payload[NRF24_PAYLOAD_LENGTH];

        nRF24_payload[0] = length;
        for (uint8_t i = 0; i < length; i++)
        {
            *(((uint8_t *)&nRF24_payload) + i + 1) = buffer[i];
        }
        nRF24_TXResult tx_res = nRF24_TransmitPacket(nRF24_payload, NRF24_PAYLOAD_LENGTH);

        // uint8_t otx = nRF24_GetRetransmitCounters();
        // uint8_t otx_plos_cnt = (otx & nRF24_MASK_PLOS_CNT) >> 4; // packets lost counter
        // uint8_t otx_arc_cnt  = (otx & nRF24_MASK_ARC_CNT); // auto retransmissions counter

        switch (tx_res)
        {
        case nRF24_TX_SUCCESS:
            // UART_SendStr("OK");
            return true;
        case nRF24_TX_TIMEOUT:
            // UART_SendStr("TIMEOUT");
            break;
        case nRF24_TX_MAXRT:
            // UART_SendStr("MAX RETRANSMIT");
            // packets_lost += otx_plos_cnt;
            nRF24_ResetPLOS();
            break;
        default:
            // UART_SendStr("ERROR");
            break;
        }

        return false;
    }

#if NRF24_RX_BUFFER_LENGTH > 0
    nRF24 nrf24;
#endif
}
