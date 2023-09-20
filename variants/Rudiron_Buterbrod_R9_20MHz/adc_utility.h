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

#ifndef ADC_UTILITY_H
#define ADC_UTILITY_H

#include "rudiron/gpio.h"
#include "rudiron/adc_types.h"

namespace Rudiron
{
    class ADC_Utility
    {
    public:
        static ADC_ChannelName getChannelName(PortPinName pinName);
    };
}

#endif // ADC_UTILITY_H
