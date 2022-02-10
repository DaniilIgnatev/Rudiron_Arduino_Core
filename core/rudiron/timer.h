/*
    This file is part of DIBotQBS.

    DIBotQBS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TIMER_H
#define TIMER_H


#include "timerutility.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace Rudiron {

    class Timer {
    public:

        static bool hasTimerForPin(PortPinName pinName);


        static TimerName getTimerNameForPin(PortPinName pinName);

    protected:

        TimerName name;


        MDR_TIMER_TypeDef *MDR_TIMER_BASE;


        uint32_t RST_CLK_PCLK;


        uint16_t prescaler;


        uint16_t ARR;

    public:
        explicit Timer(TimerName name);


        void start();


        void stop();


    protected:

        PORT_InitTypeDef PWM_getInitTypeDef(PortPinName pinName, PORT_FUNC_TypeDef func);


        void PWM_initPin(PortPinName pinName);


        void PWM_activateChannel(PortPinName pinName, uint8_t percentage, bool withNegative);

    public:

        void PWM_setup(uint16_t frequency = 500);


        void PWM_start(PortPinName pinName, uint8_t percentage);


        void PWM_start(PortPinName pinName, PortPinName invertedPinName, uint8_t percentage);


        void PWM_stop(PortPinName pinName);

    };

}

#ifdef __cplusplus
}
#endif

#endif // TIMER_H
