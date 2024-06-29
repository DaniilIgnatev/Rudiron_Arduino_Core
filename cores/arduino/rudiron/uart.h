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

#ifndef UART_H
#define UART_H

#include "config.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"

#include "rudiron/gpio.h"
#include "uart_types.h"
#include "Stream.h"

namespace Rudiron
{
    /// Не использовать с переменными типа auto, будет ошибка
    class UART : public Stream
    {
    private:
        MDR_UART_TypeDef *MDR_UART;

        uint32_t RST_CLK_PCLK_UART;

        PortPinName RX_PIN;

        PortPinName TX_PIN;

        PORT_InitTypeDef RX_PortInit;

        PORT_InitTypeDef TX_PortInit;

        IRQn_Type UART_IRQn;

        UART_BUFFER_INDEX_T *_rx_buffer_head;

        UART_BUFFER_INDEX_T *_rx_buffer_tail;

        uint8_t *_rx_buffer;

    public:
        explicit UART(
            MDR_UART_TypeDef *MDR_UART,
            uint32_t RST_CLK_PCLK_UART,
            PortPinName RX_PIN,
            PORT_InitTypeDef RX_PortInit,
            PortPinName TX_PIN,
            PORT_InitTypeDef TX_PortInit,
            IRQn_Type UART_IRQn = IRQn_Type::SysTick_IRQn,
            UART_BUFFER_INDEX_T *_rx_buffer_head = nullptr,
            UART_BUFFER_INDEX_T *_rx_buffer_tail = nullptr,
            uint8_t *_rx_buffer = nullptr);

        bool begin(
            uint32_t baudRate = 9600,
            uint16_t UART_WordLength = UART_WordLength8b,
            uint16_t UART_Parity = UART_Parity_No,
            uint16_t UART_StopBits = UART_StopBits1);

        void end();

        virtual int available(void) override;

        virtual int peek(void) override;

        virtual int read(void) override;

        virtual int availableForWrite() override;

        virtual void flush() override;

        virtual size_t write(uint8_t byte) override;

        using Print::write;

        operator bool() { return true; }

        static UART &getUART1();

        static UART &getUART2();
    };
}

#endif // UART_H
