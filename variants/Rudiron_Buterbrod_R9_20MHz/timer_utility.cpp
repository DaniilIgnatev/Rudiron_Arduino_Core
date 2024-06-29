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
// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#include "timer_utility.h"

namespace Rudiron
{
    int TimerUtility::channelToNumber(TimerChannelName channelName)
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
        else
        {
            return (int)Timer_Channel_None;
        }
    }

    TimerName TimerUtility::getTimerName(TimerChannelName channelName)
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

    TimerChannel_Descriptor TimerUtility::getTimerChannel(PortPinName pinName)
    {
        TimerChannel_Descriptor res;
        res.pinName = pinName;
        res.has = true;

        switch (pinName)
        {
        case PORT_PIN_A1:
            res.channelName = Timer1_Channel_1;
            break;
        case PORT_PIN_A2:
            res.channelName = Timer1_Channel_1; // negative
            break;
        case PORT_PIN_A3:
            res.channelName = Timer1_Channel_2;
            break;
        case PORT_PIN_A4:
            res.channelName = Timer1_Channel_2; // negative
            break;
        case PORT_PIN_A5:
            res.channelName = Timer1_Channel_3;
            break;
        case PORT_PIN_B0:
            res.channelName = Timer3_Channel_1;
            break;
        case PORT_PIN_B1:
            res.channelName = Timer3_Channel_1; // negative
            break;
        case PORT_PIN_B5:
            res.channelName = Timer3_Channel_3;
            break;
        case PORT_PIN_B6:
            res.channelName = Timer3_Channel_3; // negative
            break;
        case PORT_PIN_B7:
            res.channelName = Timer3_Channel_4;
            break;
        case PORT_PIN_B8:
            res.channelName = Timer3_Channel_4; // negative
            break;
        case PORT_PIN_C2:
            res.channelName = Timer3_Channel_1;
            break;
        case PORT_PIN_D0:
            res.channelName = Timer1_Channel_1;
            break;
        case PORT_PIN_D1:
            res.channelName = Timer1_Channel_1; // negative
            break;
        case PORT_PIN_D2:
            res.channelName = Timer3_Channel_2;
            break;
        case PORT_PIN_D3:
            res.channelName = Timer3_Channel_2; // negative
            break;
        case PORT_PIN_E0:
            res.channelName = Timer2_Channel_1;
            break;
        case PORT_PIN_E1:
            res.channelName = Timer2_Channel_1; // negative
            break;
        case PORT_PIN_E2:
            res.channelName = Timer2_Channel_3;
            break;
        case PORT_PIN_E3:
            res.channelName = Timer2_Channel_3; // negative
            break;
        case PORT_PIN_E6:
            res.channelName = Timer3_Channel_3;
            break;
        case PORT_PIN_E7:
            res.channelName = Timer3_Channel_3; // negative
            break;
        default:
            res.has = false;
            res.channelName = Timer_Channel_None;
            break;
        }

        res.timer_name = getTimerName(res.channelName);
        res.channelNum = channelToNumber(res.channelName);

        return res;
    }

    PORT_InitTypeDef
    TimerUtility::getChannelInit(PortPinName pinName, PORT_InitTypeDef PWMInit_MAIN, PORT_InitTypeDef PWMInit_ALTER,
                                 PORT_InitTypeDef PWMInit_OVERRID)
    {
        switch (pinName)
        {
        case PORT_PIN_A1:
            return PWMInit_ALTER;
        case PORT_PIN_A2:
            return PWMInit_ALTER;
        case PORT_PIN_A3:
            return PWMInit_ALTER;
        case PORT_PIN_A4:
            return PWMInit_ALTER;
        case PORT_PIN_A5:
            return PWMInit_ALTER;
        case PORT_PIN_B0:
            return PWMInit_ALTER;
        case PORT_PIN_B1:
            return PWMInit_ALTER;
        case PORT_PIN_B5:
            return PWMInit_OVERRID;
        case PORT_PIN_B6:
            return PWMInit_OVERRID;
        case PORT_PIN_B7:
            return PWMInit_OVERRID;
        case PORT_PIN_B8:
            return PWMInit_OVERRID;
        case PORT_PIN_C2:
            return PWMInit_ALTER;
        case PORT_PIN_D0:
            return PWMInit_MAIN;
        case PORT_PIN_D1:
            return PWMInit_MAIN;
        case PORT_PIN_D2:
            return PWMInit_OVERRID;
        case PORT_PIN_D3:
            return PWMInit_OVERRID;
        case PORT_PIN_E0:
            return PWMInit_ALTER;
        case PORT_PIN_E1:
            return PWMInit_ALTER;
        case PORT_PIN_E2:
            return PWMInit_ALTER;
        case PORT_PIN_E3:
            return PWMInit_ALTER;
        case PORT_PIN_E6:
            return PWMInit_OVERRID;
        case PORT_PIN_E7:
            return PWMInit_OVERRID;
        default:
            // ошибка!!!
            return PWMInit_MAIN;
        }
    }

