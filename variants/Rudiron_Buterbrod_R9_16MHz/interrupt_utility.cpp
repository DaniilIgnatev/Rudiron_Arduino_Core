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

#include "interrupt_utility.h"
#include "rudiron/interrupt_types.h"
#include "rudiron/gpio.h"

#define EXT_INT1_NUMBER BUTTON_BUILTIN_1
#define EXT_INT2_NUMBER BUTTON_BUILTIN_2
#define EXT_INT4_NUMBER BUTTON_BUILTIN_3

int digitalPinToInterrupt(int pin)
{
    switch (pin)
    {
    case EXT_INT1_NUMBER:
        return (int)Rudiron::EXT_INT1_Interrupt;
    case EXT_INT2_NUMBER:
        return (int)Rudiron::EXT_INT2_Interrupt;
    case EXT_INT4_NUMBER:
        return (int)Rudiron::EXT_INT4_Interrupt;
    default:
        return (int)Rudiron::Interrupt_None;
    }

