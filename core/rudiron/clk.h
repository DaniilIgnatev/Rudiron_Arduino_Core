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

#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_eeprom.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ALL_PORTS_CLK (RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | \
    RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD | \
    RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTF)

extern uint32_t _millis;

namespace Rudiron {

    class CLK {
    private:
        static void updateLatency(bool external, uint32_t RST_CLK_CPU_PLLmul);

        static void runHSE(uint32_t RST_CLK_CPU_PLLmul);


        static void runHSI(uint32_t RST_CLK_CPU_PLLmul);

        static void IRQ_INIT();

    public:
        static void Initialize();

    public:
        static inline uint32_t millis() {
            return _millis;
        }


        static inline unsigned long micros(void) {
            return CLK::millis() * 1000;
        }


        static inline RST_CLK_FreqTypeDef getCLKDescriptor() {
            RST_CLK_FreqTypeDef RST_CLK_Clocks;
            RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
            return RST_CLK_Clocks;
        }


        static void Delay_Init(void);


        static void delay(uint32_t ms);


        static inline void delayMicroseconds(uint32_t us) {
            CLK::delay(us / 1000);
        }
    };
}


#ifdef __cplusplus
}
#endif

#endif // PORT_H
