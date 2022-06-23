/*
This file is part of Arduino_Core_Rudiron.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef UART_H
#define UART_H


#include "MDR_config.h"
#include "MDR32Fx.h"
#include "MDR_uart.h"
#include "MDR_port.h"
#include "MDR_rst_clk.h"

#include "rudiron/gpio.h"
#include "Stream.h"

namespace Rudiron {

    class UART : public Stream {
    private:
        MDR_UART_TypeDef* MDR_UART;

        uint32_t RST_CLK_PCLK_UART;

        PortPinName RX_PIN;

        PortPinName TX_PIN;

        PORT_InitTypeDef RX_PortInit;

        PORT_InitTypeDef TX_PortInit;

        IRQn_Type UART_IRQn;

        tx_buffer_index_t *_rx_buffer_head;

        tx_buffer_index_t *_rx_buffer_tail;

        int *_rx_buffer;

    public:
        explicit UART(
            MDR_UART_TypeDef* MDR_UART,
            uint32_t RST_CLK_PCLK_UART,
            PortPinName RX_PIN,
            PORT_InitTypeDef RX_PortInit,
            PortPinName TX_PIN,
            PORT_InitTypeDef TX_PortInit,
            IRQn_Type UART_IRQn = IRQn_Type::SysTick_IRQn,
            tx_buffer_index_t *_rx_buffer_head = nullptr,
            tx_buffer_index_t *_rx_buffer_tail = nullptr,
            int *_rx_buffer = nullptr
            );


        bool begin(
                uint32_t baudRate = 9600,
                uint16_t UART_WordLength = UART_WordLength8b,
                uint16_t UART_Parity = UART_Parity_No,
                uint16_t UART_StopBits = UART_StopBits1
                );


        void end();


        virtual int available(void);


        virtual int peek(void);


        virtual int read(void);


        int availableForWrite();


        void flush();


        virtual size_t write(uint8_t byte);


        using Print::write;


        operator bool() { return true; }


        static UART* getUART1();


        static UART* getUART2();
    };
}

#endif // UART_H
