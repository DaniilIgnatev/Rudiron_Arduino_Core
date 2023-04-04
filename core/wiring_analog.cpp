/*
  wiring_analog.c - analog input and output
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
  Modified for Arduino_Core_Rudiron by Daniil Ignatev on 04.04.2023
*/

#include "wiring_private.h"
#include "pins_arduino.h"

void analogReference(uint8_t mode)
{
}

int analogRead(uint8_t pin)
{
  Rudiron::PortPinName pinName = Rudiron::GPIO::pinMap[pin];
  ADCResult result = Rudiron::ADC::readPin(pinName);

  if (!result.valid)
  {
    return -1;
  }

  return result.value;
}

// Вывод на пин ШИМ
// pin -- номер вывода
// val -- степень заполнения ШИМ от 0 до 255
void analogWrite(uint8_t pin, int val)
{
  Rudiron::PortPinName pinName = Rudiron::GPIO::pinMap[pin];
  Rudiron::Timer *timer = Rudiron::Timer::getTimerForPinName(pinName);

  if (timer != nullptr)
  {
    pinMode(pin, OUTPUT);
    timer->PWM_setup(analogWrite_Frequency);
    long pwm_val = map(val, 0, 255, 0, 1000);
    timer->PWM_start(pinName, pwm_val);
  }
}
