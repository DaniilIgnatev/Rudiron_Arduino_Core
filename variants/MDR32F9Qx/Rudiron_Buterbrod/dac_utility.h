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

#ifndef DAC_UTILITY_H
#define DAC_UTILITY_H

#include "rudiron/gpio.h"
#include "MDR32Fx.h"

namespace Rudiron
{
    // Канал ЦАП
    typedef enum
    {
        DAC_Channel_None = -1,
        DAC_Channel_0 = 0,
        DAC_Channel_1,
    } DAC_ChannelName;

    class DAC_Utility
    {
    public:
        static DAC_ChannelName getChannelName(PortPinName pinName);
    };
}

#endif // DAC_UTILITY_H
