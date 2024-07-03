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

#ifndef GPIO_TYPES_H
#define GPIO_TYPES_H

namespace Rudiron
{
    typedef enum
    {
        PORT_A,
        PORT_B,
        PORT_C,
        PORT_D,
        PORT_E,
        PORT_F
    } PortName;

    typedef enum
    {
        PORT_PIN_NONE,
        PORT_PIN_A0,
        PORT_PIN_A1,
        PORT_PIN_A2,
        PORT_PIN_A3,
        PORT_PIN_A4,
        PORT_PIN_A5,
        PORT_PIN_A6,
        PORT_PIN_A7,

        PORT_PIN_B0,
        PORT_PIN_B1,
        PORT_PIN_B2,
        PORT_PIN_B3,
        PORT_PIN_B4,
        PORT_PIN_B5,
        PORT_PIN_B6,
        PORT_PIN_B7,
        PORT_PIN_B8,
        PORT_PIN_B9,
        PORT_PIN_B10,

        PORT_PIN_C0,
        PORT_PIN_C1,
        PORT_PIN_C2,

        PORT_PIN_D0,
        PORT_PIN_D1,
        PORT_PIN_D2,
        PORT_PIN_D3,
        PORT_PIN_D4,
        PORT_PIN_D5,
        PORT_PIN_D6,
        PORT_PIN_D7,

        PORT_PIN_E0,
        PORT_PIN_E1,
        PORT_PIN_E2,
        PORT_PIN_E3,
        PORT_PIN_E4,
        PORT_PIN_E5,
        PORT_PIN_E6,
        PORT_PIN_E7,

        PORT_PIN_F0,
        PORT_PIN_F1,
        PORT_PIN_F2,
        PORT_PIN_F3
    } PortPinName;
}

#endif // GPIO_TYPES_H
