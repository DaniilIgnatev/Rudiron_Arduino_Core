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

#ifndef TIMERUTILITY_H
#define TIMERUTILITY_H

#include "MDR_timer.h"
#include "gpio.h"

namespace Rudiron
{
    typedef enum
    {
        None,
        Timer1,
        Timer2,
        Timer3
    } __attribute__((__packed__)) TimerName;

    typedef enum
    {
        Timer1_Channel1,
        Timer1_Channel2,
        Timer1_Channel3,
        Timer1_Channel4,
        Timer2_Channel1,
        Timer2_Channel2,
        Timer2_Channel3,
        Timer2_Channel4,
        Timer3_Channel1,
        Timer3_Channel2,
        Timer3_Channel3,
        Timer3_Channel4
    } __attribute__((__packed__)) TimerChannelName;

    struct TimerChannel_Descriptor
    {
        bool has = false;

        PortPinName pinName = PORT_PIN_NONE;

        TimerChannelName channelName = Timer1_Channel1;

        TimerName timer = Timer1;

        uint16_t channelNum = 0;
    };

    class TimerUtility
    {
    private:
        static int channelToNumber(TimerChannelName channelName)
        {
            if (channelName >= Timer3_Channel1)
            {
                return (int)(channelName - Timer3_Channel1);
            }
            else if (channelName >= Timer2_Channel1)
            {
                return (int)(channelName - Timer2_Channel1);
            }
            else
            {
                return (int)(channelName - Timer1_Channel1);
            }
        }

    public:
        static TimerName getTimerName(TimerChannelName channelName)
        {
            if (channelName >= Timer3_Channel1)
            {
                return Timer3;
            }
            if (channelName >= Timer2_Channel1)
            {
                return Timer2;
            }

            return Timer1;
        }

        static TimerChannel_Descriptor getTimerChannel(PortPinName pinName);

        static PORT_InitTypeDef getChannelInit(PortPinName pinName, PORT_InitTypeDef PWMInit_MAIN, PORT_InitTypeDef PWMInit_ALTER,
                                               PORT_InitTypeDef PWMInit_OVERRID);
    };

}

#endif // TIMERUTILITY_H
