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
  Modified for Rudiron_Arduino_Core by Daniil Ignatev on 08.03.2022
*/

#include "wiring_private.h"
#include "pins_arduino.h"


void analogReference(uint8_t mode)
{
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.

}

int analogRead(uint8_t pin)
{
	uint8_t low, high;

	// combine the two bytes
	return (high << 8) | low;
}

// Right now, PWM output only works on the pins with
// hardware support.
void analogWrite(uint8_t pin, int val)
{
  PortPinName pinName = GPIO::pinMap[pin];
  Timer *timer = Timer::getTimerForPinName(pinName);

  if (timer != nullptr)
  {
    pinMode(pin, OUTPUT);
    timer->start();
    timer->PWM_setup(analogWrite_Frequency);
    timer->PWM_start(pinName, val);
  }
}
