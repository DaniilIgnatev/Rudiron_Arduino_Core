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

#include "SPI.h"
#include "rudiron/ssp.h"

#define spi_instance Rudiron::SSP::getSSP1()

uint8_t SPIClass::initialized = 0;
uint8_t SPIClass::interruptMode = 0;
uint8_t SPIClass::interruptMask = 0;
uint8_t SPIClass::interruptSave = 0;
#ifdef SPI_TRANSACTION_MISMATCH_LED
uint8_t SPIClass::inTransactionFlag = 0;
#endif

SPISettings SPIClass::lastSettings;

void SPIClass::begin()
{
  uint16_t SPH = lastSettings.dataMode % 2 == 0 ? SSP_SPH_1Edge : SSP_SPH_2Edge;
  uint16_t SPO = lastSettings.dataMode < 2 ? SSP_SPO_Low : SSP_SPO_High;

  spi_instance.begin(lastSettings.clock, SPH, SPO, SSP_WordLength8b, SSP_FRF_SPI_Motorola, SSP_HardwareFlowControl_SSE);
}

void SPIClass::end()
{
  spi_instance.end();
}

void SPIClass::beginTransaction(SPISettings settings)
{
  lastSettings = settings;
  uint16_t SPH = settings.dataMode % 2 == 0 ? SSP_SPH_1Edge : SSP_SPH_2Edge;
  uint16_t SPO = settings.dataMode < 2 ? SSP_SPO_Low : SSP_SPO_High;

  spi_instance.begin(settings.clock, SPH, SPO, SSP_WordLength8b, SSP_FRF_SPI_Motorola, SSP_HardwareFlowControl_SSE);
}

// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
uint8_t SPIClass::transfer(uint8_t data)
{
  return spi_instance.read_write(data);
}

uint16_t SPIClass::transfer16(uint16_t data)
{
  return spi_instance.read_write16(data);
}

void SPIClass::transfer(void *buf, size_t count)
{
  spi_instance.write((const uint8_t *)buf, count);
}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
{
}

SPIClass SPI;
