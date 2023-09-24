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

#ifndef __FASTPIN_ARM_MDR32F9Qx_H
#define __FASTPIN_ARM_MDR32F9Qx_H

FASTLED_NAMESPACE_BEGIN

// Заглушка
inline int delayNanoseconds(int){
    return 0;
}

// #if defined(FASTLED_FORCE_SOFTWARE_PINS)
// #warning "Software pin support forced, pin access will be slightly slower."
// #define NO_HARDWARE_PIN_SUPPORT
// #undef HAS_HARDWARE_PIN_SUPPORT

// #else

// #endif // FASTLED_FORCE_SOFTWARE_PINS

FASTLED_NAMESPACE_END

#endif // __INC_FASTPIN_ARM_STM32
