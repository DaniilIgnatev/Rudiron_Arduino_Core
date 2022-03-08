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

#include "Stream.h"

namespace Rudiron {

    class UART : public Stream {
    public:
        explicit UART() : Stream() {}


        bool begin(uint32_t baudRate);


        void end();


        virtual int available(void);


        virtual int peek(void);


        virtual int read(void);


        int availableForWrite();


        void flush();


        virtual size_t write(uint8_t byte);


        using Print::write;


        operator bool() { return true; }
    };

}

#endif // UART_H
