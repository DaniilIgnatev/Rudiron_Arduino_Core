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
  * Modified for Arduino_Core_Rudiron by Daniil Ignatev on 13.03.2022
  */

 #include "SPI.h"

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


 void SPIClass::usingInterrupt(uint8_t interruptNumber)
 {

 }

 void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
 {

 }

 SPIClass SPI;
