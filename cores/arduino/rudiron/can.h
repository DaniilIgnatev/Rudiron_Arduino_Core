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

#ifndef CAN_H
#define CAN_H

#include "Stream.h"
#include "rudiron/can_types.h"
#include "gpio.h"

namespace Rudiron
{
    /// Потоко-ориентированный интерфейс для работы с CAN. Не использовать с переменными типа auto, будет ошибка
    class CAN : public Stream
    {
    private:
        CAN_Name can_name;

        bool activeID_extended = false;

        uint32_t activeID = 0;

        MDR_CAN_TypeDef *MDR_CAN;

        uint32_t RST_CLK_PCLK_CAN;

        IRQn CAN_IRQn;

        PortPinName RX_PIN;

        PortPinName TX_PIN;

        PORT_InitTypeDef RX_PortInit;

        PORT_InitTypeDef TX_PortInit;

        size_t transmit(const uint8_t *buffer, size_t size);

    public:
        explicit CAN(
            CAN_Name can_name,
            MDR_CAN_TypeDef *MDR_CAN,
            uint32_t RST_CLK_PCLK_CAN,
            IRQn CAN_IRQn,
            PortPinName RX_PIN,
            PORT_InitTypeDef RX_PortInit,
            PortPinName TX_PIN,
            PORT_InitTypeDef TX_PortInit);

        bool begin(uint32_t baudRate = 500000);

        void end();

        /// Переключение ID для чтения и для записи. Позволяет установить стандартный ID (11 бит) с возможностью расширения (до 28 бит). Тип пакета определяется автоматически. Возвращается статус изменения активного ID
        bool setActiveID(uint32_t standart, uint32_t extended);

        /// Переключение ID для чтения и для записи. Позволяет установить расширенный идентификатор пакета
        bool setActiveID(uint32_t extendedID);

        virtual int available(void) override;

        virtual int peek(void) override;

        virtual int read(void) override;

        using Print::write;

        virtual size_t write(uint8_t byte) override;

        virtual size_t write(const uint8_t *buffer, size_t size) override;

        operator bool() { return true; }

        static CAN &getCAN1();

        static CAN &getCAN2();
    };
}

#endif // CAN_H
