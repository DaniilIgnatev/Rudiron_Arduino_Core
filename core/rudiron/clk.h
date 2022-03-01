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

#ifndef CLK_H
#define CLK_H

#include "MDR_config.h"
#include "MDR32Fx.h"
#include "MDR_uart.h"
#include "MDR_port.h"
#include "MDR_rst_clk.h"
#include "MDR_eeprom.h"


extern uint32_t _millis;
extern uint32_t _micros;

namespace Rudiron {

    class CLK {
    private:

        static inline RST_CLK_FreqTypeDef getCLKDescriptor() {
            RST_CLK_FreqTypeDef RST_CLK_Clocks;
            RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
            return RST_CLK_Clocks;
        }


        static void updateLatency(bool external, uint32_t RST_CLK_CPU_PLLmul);


        static void runHSE(uint32_t RST_CLK_CPU_PLLmul);


        static void runHSI(uint32_t RST_CLK_CPU_PLLmul);


        static void init_irq();

    public:

        static void Initialize();


        static void init_delay();


        ///Значение переменной _millis
        static inline uint32_t millis(void) {
            return _millis;
        }


        ///Значение переменной _micros
        static inline uint32_t micros(void) {
            return _micros;
        }


        static void delay_millis(uint32_t ms);


        static void delay_micros(uint32_t us);
    };
}

#endif // PORT_H
