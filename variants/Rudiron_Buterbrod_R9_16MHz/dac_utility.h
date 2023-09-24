/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
*/

#ifndef DAC_UTILITY_H
#define DAC_UTILITY_H

#include "rudiron/gpio.h"
#include "MDR32Fx.h"
#include "rudiron/dac_types.h"

namespace Rudiron
{
    // Вспомогательный класс ЦАП
    class DAC_Utility
    {
    public:
        static DAC_ChannelName getDAC_ChannelName(PortPinName pinName);

        static DAC_Name getDAC_Name(DAC_ChannelName channelName);
    };
}

#endif // DAC_UTILITY_H
