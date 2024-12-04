/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron_MDR32F9Qx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron_MDR32F9Qx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron_MDR32F9Qx. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
*/

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#include "uart.h"
#include "clk.h"

#define IRQ_ENABLED UART_IRQn != IRQn::SysTick_IRQn

namespace Rudiron
{

    UART::UART(
        MDR_UART_TypeDef *MDR_UART,
        uint32_t RST_CLK_PCLK_UART,
        PortPinName RX_PIN,
        PORT_InitTypeDef RX_PortInit,
        PortPinName TX_PIN,
        PORT_InitTypeDef TX_PortInit,
        IRQn_Type UART_IRQn,
        UART_BUFFER_INDEX_T *_rx_buffer_head,
        UART_BUFFER_INDEX_T *_rx_buffer_tail,
        uint8_t *_rx_buffer) : Stream()
    {
        _timeout = 100;

        this->MDR_UART = MDR_UART;
        this->RST_CLK_PCLK_UART = RST_CLK_PCLK_UART;
        this->RX_PIN = RX_PIN;
        this->RX_PortInit = RX_PortInit;
        this->TX_PIN = TX_PIN;
        this->TX_PortInit = TX_PortInit;
        this->UART_IRQn = UART_IRQn;
        this->_rx_buffer_head = _rx_buffer_head;
        this->_rx_buffer_tail = _rx_buffer_tail;
        this->_rx_buffer = _rx_buffer;
    }

    bool UART::begin(
        uint32_t baudRate,
        uint16_t UART_WordLength,
        uint16_t UART_Parity,
        uint16_t UART_StopBits)
    {

        GPIO::configPin(RX_PIN, RX_PortInit);
        GPIO::configPin(TX_PIN, TX_PortInit);

        /* Enables the CPU_CLK clock*/
        RST_CLK_PCLKcmd(RST_CLK_PCLK_UART, ENABLE);

        /* Set the HCLK division factor*/
        UART_BRGInit(MDR_UART, CLK::getHCLKdiv());

        // EnableIRQ
        if (IRQ_ENABLED)
        {
            for (UART_BUFFER_INDEX_T i = 0; i < SERIAL_RX_BUFFER_LENGTH; i++)
            {
                _rx_buffer[i] = EndOfStream;
            }

            UART_ITConfig(MDR_UART, UART_IT_RX, ENABLE);
            NVIC_EnableIRQ(UART_IRQn);
        }

        /* Initialize UART_InitStructure */
        UART_InitTypeDef UART_InitStructure;
        UART_InitStructure.UART_BitRate = baudRate;
        UART_InitStructure.UART_WordLength = UART_WordLength;
        UART_InitStructure.UART_StopBits = UART_StopBits;
        UART_InitStructure.UART_Parity = UART_Parity;
        UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
        UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

        /* Configure UART parameters*/
        bool status = UART_Init(MDR_UART, &UART_InitStructure);

        if (status)
        {
            UART_Cmd(MDR_UART, ENABLE);
        }

        return status;
    }

    void UART::end()
    {
        UART_Cmd(MDR_UART, DISABLE);
        if (IRQ_ENABLED)
        {
            NVIC_DisableIRQ(UART_IRQn);
            UART_ITConfig(MDR_UART, UART_IT_RX, DISABLE);
        }
        RST_CLK_PCLKcmd(RST_CLK_PCLK_UART, DISABLE);
    }

    int UART::available()
    {
        if (IRQ_ENABLED)
        {
            return *_rx_buffer_tail != *_rx_buffer_head;
        }
        else
        {
            return UART_GetFlagStatus(MDR_UART, UART_FLAG_RXFF);
        }
    }

    int UART::peek()
    {
        if (IRQ_ENABLED)
        {
            return _rx_buffer[*_rx_buffer_tail];
        }
        else
        {
            if (UART_GetFlagStatus(MDR_UART, UART_FLAG_RXFF) != SET)
            {
                return EndOfStream;
            }

            return UART_ReceiveData(MDR_UART);
        }
    }

    int UART::read()
    {
        if (IRQ_ENABLED)
        {
            if (*_rx_buffer_tail == *_rx_buffer_head)
            {
                // нет данных для чтения
                return EndOfStream;
            }

            int data = _rx_buffer[*_rx_buffer_tail];
            UART_BUFFER_INDEX_T next_tail = *_rx_buffer_tail + 1;
            if (next_tail == SERIAL_RX_BUFFER_LENGTH)
            {
                next_tail = 0;
            }
            *_rx_buffer_tail = next_tail;
            return data;
        }
        else
        {
            if (UART_GetFlagStatus(MDR_UART, UART_FLAG_RXFF) != SET)
            {
                return EndOfStream;
            }

            return UART_ReceiveData(MDR_UART);
        }
    }

    int UART::availableForWrite()
    {
        return UART_GetFlagStatus(MDR_UART, UART_FLAG_TXFE);
    }

    void UART::flush()
    {
        while (UART_GetFlagStatus(MDR_UART, UART_FLAG_TXFE) != SET)
        {
        }
    }

    size_t UART::write(uint8_t byte)
    {
        /* Check TXFE flag*/
        while (UART_GetFlagStatus(MDR_UART, UART_FLAG_TXFE) != SET)
        {
        }

        /* Send Data */
        UART_SendData(MDR_UART, (uint16_t)byte);

        return true;
    };

    UART &UART::getUART1()
    {
        PORT_InitTypeDef RX_PortInit;
        RX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        RX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        RX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        RX_PortInit.PORT_PD = PORT_PD_DRIVER;
        RX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        RX_PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
        RX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        RX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        RX_PortInit.PORT_OE = PORT_OE_IN;

        PORT_InitTypeDef TX_PortInit;
        TX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        TX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        TX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        TX_PortInit.PORT_PD = PORT_PD_DRIVER;
        TX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        TX_PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
        TX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        TX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        TX_PortInit.PORT_OE = PORT_OE_OUT;

        static UART uart(MDR_UART1, RST_CLK_PCLK_UART1, PORT_PIN_A6, RX_PortInit, PORT_PIN_A7, TX_PortInit, UART1_IRQn,
                         &_uart1_rx_buffer_head, &_uart1_rx_buffer_tail, _uart1_rx_buffer);
        return uart;
    }

    UART &UART::getUART2()
    {
        PORT_InitTypeDef RX_PortInit;
        RX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        RX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        RX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        RX_PortInit.PORT_PD = PORT_PD_DRIVER;
        RX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        RX_PortInit.PORT_FUNC = PORT_FUNC_ALTER;
        RX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        RX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        RX_PortInit.PORT_OE = PORT_OE_IN;

        PORT_InitTypeDef TX_PortInit;
        TX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        TX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        TX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        TX_PortInit.PORT_PD = PORT_PD_DRIVER;
        TX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        TX_PortInit.PORT_FUNC = PORT_FUNC_ALTER;
        TX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        TX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        TX_PortInit.PORT_OE = PORT_OE_OUT;

        static UART uart(MDR_UART2, RST_CLK_PCLK_UART2, PORT_PIN_D0, RX_PortInit, PORT_PIN_D1, TX_PortInit, UART2_IRQn,
                         &_uart2_rx_buffer_head, &_uart2_rx_buffer_tail, _uart2_rx_buffer);
        return uart;
    }
}
