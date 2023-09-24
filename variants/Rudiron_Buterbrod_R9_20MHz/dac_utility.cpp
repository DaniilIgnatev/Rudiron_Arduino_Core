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

#include "dac_utility.h"

namespace Rudiron
{
    DAC_ChannelName DAC_Utility::getDAC_ChannelName(PortPinName pinName)
    {
        switch (pinName)
        {
        case PORT_PIN_E0:
            return DAC_ChannelName::DAC_Channel_2;
        default:
            return DAC_ChannelName::DAC_Channel_None;
        }
    }

    DAC_Name DAC_Utility::getDAC_Name(DAC_ChannelName channelName)
    {
        switch (channelName)
        {
        case DAC_Channel_2:
            return DAC_Name::DAC_2;
        default:
            return DAC_Name::DAC_None;
        }
    }
}
