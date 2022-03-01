/*
This file is part of DIBotQBS.

DIBotQBS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DIBotQBS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DIBotQBS.  If not, see <https://www.gnu.org/licenses/>.
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

    class Can {
    private:
        uint8_t tx_buf = 0;
        uint8_t rx_buf = 1;

    public:
        Can();

        bool begin();


        void end();


        void write();


        void read();
    };

}


#endif // CAN_H
