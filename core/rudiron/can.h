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


#include "Stream.h"
#include "MDR32Fx.h"
#include "MDR_config.h"
#include "MDR_can.h"
#include "MDR_rst_clk.h"
#include "gpio.h"


#define CANTEST 1

namespace Rudiron {

    class CAN : public Stream
    {
    private:

        uint32_t active_ID = 0;

        MDR_CAN_TypeDef* MDR_CAN;

        uint32_t RST_CLK_PCLK_CAN;

        PortPinName RX_PIN;

        PortPinName TX_PIN;

        PORT_InitTypeDef RX_PortInit;

        PORT_InitTypeDef TX_PortInit;

    public:

        explicit CAN(
            MDR_CAN_TypeDef* MDR_CAN,
            uint32_t RST_CLK_PCLK_CAN,
            PortPinName RX_PIN,
            PORT_InitTypeDef RX_PortInit,
            PortPinName TX_PIN,
            PORT_InitTypeDef TX_PortInit
            );


        bool begin(uint32_t baudRate = 500000);


        void end();


        void setActiveID(uint32_t ID);


        void write(CAN_TxMsgTypeDef &TxMsg);


        virtual int available(void);


        virtual int peek(void);


        virtual int read(void);


        virtual size_t write(uint8_t byte);


        using Print::write;


        operator bool() { return true; }

    };
}


#endif // CAN_H
