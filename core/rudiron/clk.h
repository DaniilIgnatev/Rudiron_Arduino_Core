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

namespace Rudiron
{
    enum CLK_Speed
    {
        low,
        medium,
        high
    };

    extern CLK_Speed _CLK_Speed;

    class CLK
    {
    private:
        static inline RST_CLK_FreqTypeDef getCLKDescriptor()
        {
            RST_CLK_FreqTypeDef RST_CLK_Clocks;
            RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
            return RST_CLK_Clocks;
        }

        static void init_irq();

        static void updateLatency(bool external, uint32_t RST_CLK_CPU_PLLmul);

        static void runHSE(uint32_t RST_CLK_CPU_PLLmul);

        static void runHSI(uint32_t RST_CLK_CPU_PLLmul);

        static void init_delay();

    public:
        static void initialise();

        ///Копия значения _CLK_Speed
        static inline CLK_Speed getSpeed()
        {
            return _CLK_Speed;
        }

        static void setSpeed(CLK_Speed newValue = CLK_Speed::low);

        static inline uint32_t getCPUFrequency()
        {
            return getCLKDescriptor().CPU_CLK_Frequency;
        }

        static inline uint32_t getCPU_Multiplier();

        static uint32_t getHCLKdiv();

        ///Копия значения _millis
        static inline uint32_t millis(void)
        {
            return _millis;
        }

        ///Копия значения _micros
        static inline uint32_t micros(void)
        {
            return _micros;
        }

        static void delay_millis(uint32_t ms);

        static void delay_micros(uint32_t us);
    };
}

#endif // PORT_H
