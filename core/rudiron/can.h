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



#ifndef CAN_H
#define CAN_H


#include "Print.h"
#include "MDR32Fx.h"
#include "MDR_config.h"
#include "MDR_can.h"
#include "MDR_rst_clk.h"
#include "MDR_port.h"


namespace Rudiron {

    class CAN
    {
    private:
        uint8_t tx_buf_number = 0;
        uint8_t rx_buf_number = 1;

    public:
        CAN();

        bool begin();


        void end();


        void write(CAN_DataTypeDef &data);


        bool read(CAN_RxMsgTypeDef &data, bool wait = true, uint32_t timeout = 0xFFF);
    };
}


#endif // CAN_H
