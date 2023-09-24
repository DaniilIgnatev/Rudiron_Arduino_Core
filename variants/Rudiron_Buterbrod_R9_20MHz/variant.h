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

#ifndef ARDUINO_CORE_RUDIRON_VARIANT_H
#define ARDUINO_CORE_RUDIRON_VARIANT_H

#include "HardwareSerial.h"

extern HardwareSerial Serial;
#define HAVE_HWSERIAL0

extern HardwareSerial Serial1;
#define HAVE_HWSERIAL1

#endif // ARDUINO_CORE_RUDIRON_VARIANT_H
