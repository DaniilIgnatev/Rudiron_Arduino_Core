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

#include "gpio_map.h"
#include "rudiron/gpio.h"
#include "pins_arduino.h"

namespace Rudiron
{
#ifdef Rudiron_Buterbrod

#ifdef revision_2
        const PortPinName GPIO::pinMap[NUM_DIGITAL_PINS]{
            PORT_PIN_F3,  // 0
            PORT_PIN_F2,  // 1
            PORT_PIN_F1,  // 2
            PORT_PIN_F0,  // 3
            PORT_PIN_A1,  // 4
            PORT_PIN_A2,  // 5
            PORT_PIN_A3,  // 6
            PORT_PIN_A4,  // 7
            PORT_PIN_A5,  // 8
            PORT_PIN_A6,  // 9
            PORT_PIN_A7,  // 10
            PORT_PIN_B8,  // 11
            PORT_PIN_B7,  // 12
            PORT_PIN_B6,  // 13
            PORT_PIN_B5,  // 14
            PORT_PIN_B4,  // 15
            PORT_PIN_B3,  // 16
            PORT_PIN_B0,  // 17
            PORT_PIN_C0,  // 18
            PORT_PIN_C1,  // 19
            PORT_PIN_C2,  // 20
            PORT_PIN_D6,  // 21
            PORT_PIN_D5,  // 22
            PORT_PIN_D3,  // 23
            PORT_PIN_D2,  // 24
            PORT_PIN_D4,  // 25
            PORT_PIN_D7,  // 26
            PORT_PIN_E0,  // 27
            PORT_PIN_E1,  // 28
            PORT_PIN_E2,  // 29
            PORT_PIN_E3,  // 30
            PORT_PIN_B10, // 31
            PORT_PIN_B9,  // 32
            PORT_PIN_E6,  // 33
            PORT_PIN_E7,  // 34
            PORT_PIN_A0,  // 35
            PORT_PIN_D0,  // 36
            PORT_PIN_D1,  // 37
        };

        const PortPinName GPIO::pinADCMap[NUM_ANALOG_INPUTS]{
            PORT_PIN_D6, // A0
            PORT_PIN_D5, // A1
            PORT_PIN_D3, // A2
            PORT_PIN_D2, // A3
            PORT_PIN_D4, // A4
            PORT_PIN_D7, // A5
        };
#endif
#endif
}
