#include "nrf24.h"
#include "Arduino.h"

void interrupt_callback()
{
    nRF24_bufferPayload();

    nRF24_ClearIRQFlags();
    nRF24_maskIRQ(0, 0, 0);
}

namespace Rudiron
{
    const uint8_t nRF24::nRF24_DEFAULT_ADDR[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xF0};

    nRF24::nRF24() : Stream()
    {
        _timeout = 100; // надежный таймаут чтения сообщений неопределенной длины
    }

    bool nRF24::begin(bool receiver, uint8_t RFChannel, uint8_t nRF24_TXPWR, const uint8_t address[5], uint8_t nRF24_DR, uint8_t nRF24_CRC, uint8_t nRF24_ARD, uint8_t nRF24_ARC)
    {
        if (RFChannel > 124)
        {
            return false;
        }

        isReceiver = receiver;
        nRF24_GPIO_Init();
        Rudiron::SSP::getSSP2().begin(4000000, SSP_SPH_1Edge, SSP_SPO_Low, SSP_WordLength8b, SSP_FRF_SPI_Motorola, SSP_HardwareFlowControl_SSE);

#ifdef NRF24_USE_INTERRUPT
        if (isReceiver)
        {
            pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
            attachInterrupt(digitalPinToInterrupt(BUTTON_BUILTIN_2), interrupt_callback);
        }
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

        nRF24_SetRFChannel(RFChannel);

        // Set data rate
        nRF24_SetDataRate(nRF24_DR);

        // Set CRC scheme
        nRF24_SetCRCScheme(nRF24_CRC);

        // Set address width, its common for all pipes (RX and TX)
        nRF24_SetAddrWidth(5);

        if (isReceiver)
        {
            // Адрес приемника
            nRF24_SetAddr(nRF24_PIPE0, nRF24_DEFAULT_ADDR);

            // Параметры приемника
            nRF24_SetRXPipe(nRF24_PIPE0, nRF24_AA_ON, NRF24_PAYLOAD_LENGTH);

            // Set TX power
            nRF24_SetTXPower(nRF24_TXPWR);

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
            // Адрес получателя
            nRF24_SetAddr(nRF24_PIPETX, nRF24_DEFAULT_ADDR);

            // Set TX power
            nRF24_SetTXPower(nRF24_TXPWR);

            // Configure auto retransmit: nRF24_ARC retransmissions with pause of nRF24_ARD in between
            nRF24_SetAutoRetr(nRF24_ARD, nRF24_ARC);

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
        Rudiron::SSP::getSSP2().end();
        nRF24_GPIO_DeInit();

#ifdef NRF24_USE_INTERRUPT
        detachInterrupt(digitalPinToInterrupt(BUTTON_BUILTIN_2));
#endif
    }

    int nRF24::available()
    {
        if (!isReceiver)
        {
            return 0;
        }

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
        nRF24_bufferPayload();
#endif

        // Чтение кольцевого буфера
        if (_nrf24_rx_buffer_tail == _nrf24_rx_buffer_head)
        {
            // нет данных для чтения
            return EndOfStream;
        }

        int data = _nrf24_rx_buffer[_nrf24_rx_buffer_tail];
        NRF24_BUFFER_INDEX_T next_tail = _nrf24_rx_buffer_tail + 1;
        if (next_tail == NRF24_RX_BUFFER_LENGTH)
        {
            next_tail = 0;
        }
        _nrf24_rx_buffer_tail = next_tail;
        return data;
    }

    int nRF24::availableForWrite()
    {
        if (isReceiver)
        {
            return 0;
        }

        nRF24_FlushTX();
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
        return transmit(&byte, 1);
    }

    /// @brief Возвращает число отправленных байт
    /// @param buffer
    /// @param size
    /// @return
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

        const nRF24_TXResult tx_res = nRF24_TransmitPacket(nRF24_payload, NRF24_PAYLOAD_LENGTH);

        switch (tx_res)
        {
        case nRF24_TX_SUCCESS:
            return length;
        case nRF24_TX_TIMEOUT:
            break;
        case nRF24_TX_MAXRT:
            nRF24_ResetPLOS();
            return length;
        default:
            break;
        }

        return 0;
    }
}

#if NRF24_RX_BUFFER_LENGTH > 0
Rudiron::nRF24 nrf24;
#endif