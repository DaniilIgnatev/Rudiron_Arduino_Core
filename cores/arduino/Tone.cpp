/* Tone.cpp

  A Tone Generator Library

  Written by Brett Hagman

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Version Modified By Date     Comments
------- ----------- -------- --------
0001    B Hagman    09/08/02 Initial coding
0002    B Hagman    09/08/18 Multiple pins
0003    B Hagman    09/08/18 Moved initialization from constructor to begin()
0004    B Hagman    09/09/26 Fixed problems with ATmega8
0005    B Hagman    09/11/23 Scanned prescalars for best fit on 8 bit timers
                    09/11/25 Changed pin toggle method to XOR
                    09/11/25 Fixed timer0 from being excluded
0006    D Mellis    09/12/29 Replaced objects with functions
0007    M Sproul    10/08/29 Changed #ifdefs from cpu to register
0008    S Kanemoto  12/06/22 Fixed for Leonardo by @maris_HY
0009    J Reucker   15/04/10 Issue #292 Fixed problems with ATmega8 (thanks to Pete62)
0010    jipp        15/04/13 added additional define check #2923
*************************************************/

// Modified for Arduino_Core_Rudiron_MDR32F9Qx by Daniil Ignatev on 08.03.2022

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#include "Arduino.h"
#include "pins_arduino.h"
#include "wiring_private.h"

// frequency (in hertz) and duration (in milliseconds).
void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
    Rudiron::PortPinName pinName = Rudiron::GPIO::get_rudiron_gpio(pin);

    if (Rudiron::Timer::hasTimer_for_pinName(pinName))
    {
        Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_pinName(pinName);

        pinMode(pin, OUTPUT);

        timer.setup(frequency);
        timer.PWM_start(pinName, 500, 1000);

        if (duration)
        {
            delay(duration);
            noTone(pin);
        }
    }
}

void noTone(uint8_t pin)
{
    Rudiron::PortPinName pinName = Rudiron::GPIO::get_rudiron_gpio(pin);

    if (Rudiron::Timer::hasTimer_for_pinName(pinName))
    {
        Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_pinName(pinName);
        timer.PWM_stop(pinName);
    }
}
