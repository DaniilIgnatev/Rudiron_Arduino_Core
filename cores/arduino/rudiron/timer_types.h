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

#ifndef TIMER_TYPES_H
#define TIMER_TYPES_H

#include "MDR32F9Qx_timer.h"
#include "rudiron/gpio.h"

namespace Rudiron
{
    typedef enum
    {
        Timer_None = -1,
        Timer_1 = 0,
        Timer_2,
        Timer_3
    } __attribute__((__packed__)) TimerName;

    typedef enum
    {
        Timer_Channel_None = -1,
        Timer1_Channel_1 = 0,
        Timer1_Channel_2,
        Timer1_Channel_3,
        Timer1_Channel_4,
        Timer2_Channel_1,
        Timer2_Channel_2,
        Timer2_Channel_3,
        Timer2_Channel_4,
        Timer3_Channel_1,
        Timer3_Channel_2,
        Timer3_Channel_3,
        Timer3_Channel_4
    } __attribute__((__packed__)) TimerChannelName;

    struct TimerChannel_Descriptor
    {
        bool has = false;

        PortPinName pinName = PORT_PIN_NONE;

        TimerChannelName channelName = Timer_Channel_None;

        TimerName timer_name = Timer_None;

        uint16_t channelNum = 0;
    };
}

#endif // TIMER_TYPES_H
