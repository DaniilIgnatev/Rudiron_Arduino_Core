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
  Modified for Arduino_Core_Rudiron_MDR32F9Qx by Daniil Ignatev on 04.04.2023
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

#include "wiring_private.h"
#include "rudiron/adc.h"

void analogReference(uint8_t mode)
{
}

int __attribute__((optimize("O0"))) analogRead(uint8_t pin)
{
    Rudiron::PortPinName pinName = Rudiron::GPIO::get_rudiron_gpio(pin);
    Rudiron::ADCResult result = Rudiron::ADC::getADC2().read_pin_single(pinName);

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
    Rudiron::PortPinName pinName = Rudiron::GPIO::get_rudiron_gpio(pin);

    if (Rudiron::Timer::hasTimer_for_pinName(pinName))
    {
        Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_pinName(pinName);
        timer.setup(analogWrite_Frequency);
        const uint16_t width_max = 65535;
        long pwm_val = map(val, 0, 255, 0, width_max);
        timer.PWM_start(pinName, pwm_val, width_max);
    }
}
