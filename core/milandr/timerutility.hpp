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


#ifndef TIMERUTILITY_H
#define TIMERUTILITY_H


#include "MDR32F9Qx_timer.h"
#include "gpio.h"


typedef enum{
    None,
    Timer1,
    Timer2,
    Timer3
} __attribute__ ((__packed__)) TimerName;



typedef enum{
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
} __attribute__ ((__packed__)) TimerChannelName;


struct TimerChannel_PortPin{
    bool has = false;

    PortPinName pinName = PIN_NONE;

    TimerChannelName channelName = Timer1_Channel1;

    TimerName timer = Timer1;

    uint16_t channelNum = 0;
};



class TimerUtility{

private:
    static int channelToNumber(TimerChannelName channelName);

public:
    static TimerName channelToTimerName(TimerChannelName channelName);


    static TimerChannel_PortPin pinChannel(PortPinName pinName);


    static PORT_InitTypeDef getChannelInit(PortPinName pinName, PORT_InitTypeDef PWMInit_MAIN, PORT_InitTypeDef PWMInit_ALTER, PORT_InitTypeDef PWMInit_OVERRID);
};


#endif // TIMERUTILITY_H
