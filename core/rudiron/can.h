/*
This file is part of Rudiron_Arduino_Core.

Rudiron_Arduino_Core is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Rudiron_Arduino_Core is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Rudiron_Arduino_Core.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CAN_H
#define CAN_H

#include "Stream.h"
#include "MDR32Fx.h"
#include "MDR_config.h"
#include "MDR_can.h"
#include "MDR_rst_clk.h"
#include "gpio.h"

#define CANTEST 1

namespace Rudiron
{

    class CAN : public Stream
    {
    private:
        bool activeID_extended = false;

        uint32_t activeID = 0;

        MDR_CAN_TypeDef *MDR_CAN;

        uint32_t RST_CLK_PCLK_CAN;

        IRQn CAN_IRQn;

        PortPinName RX_PIN;

        PortPinName TX_PIN;

        PORT_InitTypeDef RX_PortInit;

        PORT_InitTypeDef TX_PortInit;

    public:
        explicit CAN(
            MDR_CAN_TypeDef *MDR_CAN,
            uint32_t RST_CLK_PCLK_CAN,
            IRQn CAN_IRQn,
            PortPinName RX_PIN,
            PORT_InitTypeDef RX_PortInit,
            PortPinName TX_PIN,
            PORT_InitTypeDef TX_PortInit);

        bool begin(uint32_t baudRate = 500000);

        void end();

        ///Переключение ID для чтения и для записи. Позволяет установить стандартный ID (11 бит) с возможностью расширения (до 28 бит). Тип пакета определяется автоматически. Возвращается статус изменения активного ID
        bool setActiveID(uint32_t standart, uint32_t extended);

        ///Переключение ID для чтения и для записи. Позволяет установить расширенный идентификатор пакета
        bool setActiveID(uint32_t extendedID);

        virtual int available(void);

        virtual int peek(void);

        virtual int read(void);

        size_t transmit(const uint8_t *buffer, size_t size);

        using Print::write;

        virtual size_t write(uint8_t byte);

        virtual size_t write(const uint8_t *buffer, size_t size);

        operator bool() { return true; }

        static CAN *getCAN1();

        static CAN *getCAN2();
    };
}

#endif // CAN_H
