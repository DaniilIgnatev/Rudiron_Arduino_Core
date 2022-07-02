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

#ifndef NRF24_H
#define NRF24_H

#include "MDR_config.h"
#include "Stream.h"
#include "nrf24l01.h"

namespace Rudiron
{
    ///Потоко-ориентированный интерфейс для работы с nrf24
    class nRF24: public Stream
    {
        private:
        size_t transmit(const uint8_t *buffer, uint8_t length);
    public:
        explicit nRF24();

        bool begin(bool receiver);

        void end();

        virtual int available(void) override;

        virtual int peek(void) override;

        virtual int read(void) override;

        virtual int availableForWrite() override;

        virtual void flush() override;

        virtual size_t write(uint8_t byte) override;

        virtual size_t write(const uint8_t *buffer, size_t size) override;

        using Print::write;

        operator bool() { return true; }
    };

#if NRF24_RX_BUFFER_LENGTH > 0
    extern nRF24 nrf24;
#endif
}

#endif