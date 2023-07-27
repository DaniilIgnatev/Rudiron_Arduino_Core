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

#ifndef GPIO_UTILITY_H
#define GPIO_UTILITY_H

#include "pins_arduino.h"
#include "rudiron/gpio_types.h"

namespace Rudiron
{
    class GPIOUtility
    {
    public:
        static const PortPinName pinMap[NUM_DIGITAL_PINS];
    };
}

#endif // GPIO_UTILITY_H
