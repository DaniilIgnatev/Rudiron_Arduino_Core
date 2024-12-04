/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul
  Modified for Arduino_Core_Rudiron_MDR32F9Qx by Daniil Ignatev on 08.03.2022
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

#define ARDUINO_MAIN

#include "wiring_private.h"
#include "pins_arduino.h"
#include "rudiron/gpio.h"

void pinMode(uint8_t pin, uint8_t mode)
{
    Rudiron::PortPinName pinName = Rudiron::GPIO::get_rudiron_gpio(pin);

    switch (mode)
    {
    case INPUT:
        Rudiron::GPIO::configPinInput(pinName);
        break;
    case INPUT_PULLUP:
        Rudiron::GPIO::configPinInputPullUp(pinName);
        break;
    case INPUT_PULLDOWN:
        Rudiron::GPIO::configPinInputPullDown(pinName);
        break;
    case OUTPUT:
        Rudiron::GPIO::configPinOutput(pinName);
        break;
    default:
        break;
    }
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    Rudiron::PortPinName pinName = Rudiron::GPIO::get_rudiron_gpio(pin);
    Rudiron::GPIO::writePin(pinName, (bool)val);
}

int digitalRead(uint8_t pin)
{
    Rudiron::PortPinName pinName = Rudiron::GPIO::get_rudiron_gpio(pin);
    return Rudiron::GPIO::readPin(pinName);
}
