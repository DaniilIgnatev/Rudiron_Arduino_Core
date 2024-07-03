/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Wiring project - http://wiring.uniandes.edu.co

  Copyright (c) 2004-05 Hernando Barragan

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

  Modified 24 November 2006 by David A. Mellis
  Modified 1 August 2010 by Mark Sproul
  Modified for Arduino_Core_Rudiron_MDR32F9Qx by Daniil Ignatev on 28.08.2023
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

#include <inttypes.h>
#include <stdio.h>

#include "wiring_private.h"
#include "rudiron/interrupt.h"

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode)
{
  Rudiron::Interrupt_Name interrupt_name = (Rudiron::Interrupt_Name)interruptNum;
  Rudiron::Interrupt::attach(interrupt_name, userFunc);
}

void detachInterrupt(uint8_t interruptNum)
{
  Rudiron::Interrupt_Name interrupt_name = (Rudiron::Interrupt_Name)interruptNum;
  Rudiron::Interrupt::detach(interrupt_name);
}
