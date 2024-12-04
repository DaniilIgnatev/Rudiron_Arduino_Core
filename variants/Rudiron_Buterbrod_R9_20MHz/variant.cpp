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

#include "variant.h"
#include "wiring_private.h"
#include "config.h"

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/)()) { return 0; }

void initVariant()
{
    Rudiron::CAN_RX_Package emptyPackage;
    emptyPackage.ID = 0;
    emptyPackage.Mask = 0;

    for (uint8_t i = 0; i < CAN_RX_BUFFER_LENGTH; i++)
    {
        _can_rx_buffer[i] = emptyPackage;
    }
}

void setupUSB()
{
}

HardwareSerial Serial(Rudiron::UART::getUART2());

HardwareSerial Serial1(Rudiron::UART::getUART1());

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial0_available()
{
    return Serial.available();
}

bool Serial1_available()
{
    return Serial1.available();
}
