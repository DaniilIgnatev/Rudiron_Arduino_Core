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

#ifndef TIMER_UTILITY_H
#define TIMER_UTILITY_H

#include "MDR_timer.h"
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

        TimerName timer = Timer_None;

        uint16_t channelNum = 0;
    };

    class TimerUtility
    {
    private:
        static int channelToNumber(TimerChannelName channelName)
        {
            if (channelName >= Timer3_Channel_1)
            {
                return (int)(channelName - Timer3_Channel_1);
            }
            else if (channelName >= Timer2_Channel_1)
            {
                return (int)(channelName - Timer2_Channel_1);
            }
            else if (channelName >= Timer1_Channel_1)
            {
                return (int)(channelName - Timer1_Channel_1);
            }
            else{
                return (int)Timer_Channel_None;
            }
        }

    public:
        static TimerName getTimerName(TimerChannelName channelName)
        {
            if (channelName >= Timer3_Channel_1)
            {
                return Timer_3;
            }
            if (channelName >= Timer2_Channel_1)
            {
                return Timer_2;
            }
            if (channelName >= Timer1_Channel_1)
            {
                return Timer_1;
            }

            return Timer_None;
        }

        static TimerChannel_Descriptor getTimerChannel(PortPinName pinName);

        static PORT_InitTypeDef getChannelInit(PortPinName pinName, PORT_InitTypeDef PWMInit_MAIN, PORT_InitTypeDef PWMInit_ALTER,
                                               PORT_InitTypeDef PWMInit_OVERRID);
    };

}

#endif // TIMER_UTILITY_H
