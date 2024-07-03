/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 *  Modified 2022 by Daniil Ignatev for Arduino_Core_Rudiron_MDR32F9Qx
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

#ifndef _SPI_H
#define _SPI_H

#include <Arduino.h>

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

// #define SPI_CLOCK_DIV4 0x00
// #define SPI_CLOCK_DIV16 0x01
// #define SPI_CLOCK_DIV64 0x02
// #define SPI_CLOCK_DIV128 0x03
// #define SPI_CLOCK_DIV2 0x04
// #define SPI_CLOCK_DIV8 0x05
// #define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C


class SPISettings
{
public:
  uint32_t clock;
  uint8_t bitOrder;
  uint8_t dataMode;

  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
  {
    this->clock = clock;
    this->bitOrder = bitOrder;
    this->dataMode = dataMode;
  }
  SPISettings()
  {
    this->clock = 8000000;
    this->bitOrder = MSBFIRST;
    this->dataMode = SPI_MODE0;
  }
};

class SPIClass
{
protected:
  static SPISettings lastSettings;

public:
  // Initialize the SPI library
  static void begin();

  // If SPI is used from within an interrupt, this function registers
  // that interrupt with the SPI library, so beginTransaction() can
  // prevent conflicts.  The input interruptNumber is the number used
  // with attachInterrupt.  If SPI is used from a different interrupt
  // (eg, a timer), interruptNumber should be 255.
  static void usingInterrupt(uint8_t interruptNumber);
  static void notUsingInterrupt(uint8_t interruptNumber);

  static void beginTransaction(SPISettings settings);

  // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
  static uint8_t transfer(uint8_t data);

  static uint16_t transfer16(uint16_t data);

  static void transfer(void *buf, size_t count);

  // After performing a group of transfers and releasing the chip select
  // signal, this function allows others to access the SPI bus
  static void endTransaction(void)
  {

  }

  // Disable the SPI bus
  static void end();

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setBitOrder(uint8_t bitOrder)
  {
    lastSettings.bitOrder = bitOrder;
    beginTransaction(lastSettings);
  }
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setDataMode(uint8_t dataMode)
  {
    lastSettings.dataMode = dataMode;
    beginTransaction(lastSettings);
  }
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setClockDivider(uint8_t clockDiv)
  {
  }

  // These undocumented functions should not be used.  SPI.transfer()
  // polls the hardware flag which is automatically cleared as the
  // AVR responds to SPI's interrupt
  inline static void attachInterrupt()
  {
  }
  
  inline static void detachInterrupt()
  {
  }

private:
  static uint8_t initialized;
  static uint8_t interruptMode; // 0=none, 1=mask, 2=global
  static uint8_t interruptMask; // which interrupts to mask
  static uint8_t interruptSave; // temp storage, to restore state
#ifdef SPI_TRANSACTION_MISMATCH_LED
  static uint8_t inTransactionFlag;
#endif
};

extern SPIClass SPI;

#endif
