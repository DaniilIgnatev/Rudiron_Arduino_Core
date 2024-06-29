/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2020 by Greyson Christoforo (grey@christoforo.net) to implement timeouts
  Modified 2022 by Daniil Ignatev for Arduino_Core_Rudiron_MDR32F9Qx
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

#ifndef TwoWire_h
#define TwoWire_h

#include <inttypes.h>
#include "Stream.h"
#include "Arduino.h"

#define BUFFER_LENGTH 128

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

class TwoWire : public Stream
{
private:
  // Является ли передача MOSI
  static uint8_t transmitting;
  /// Адрес целевого устройства
  static uint8_t txAddress;

  // Буфер передачи
  static uint8_t txBuffer[];
  // Номер первого байта в буфере передачи
  static uint8_t txBufferIndex;
  // Количество байт в буфере передачи
  static uint8_t txBufferLength;

  static uint8_t writeSync(uint8_t address, uint8_t *data, uint8_t length, uint8_t sendStop);

  // Буфер приема
  static uint8_t rxBuffer[];
  // Номер первого байта в буфере приема
  static uint8_t rxBufferIndex;
  // Количество байт в буфере приема
  static uint8_t rxBufferLength;

  uint8_t readSync(uint8_t address, uint8_t *data, uint8_t length, uint8_t sendStop);

public:
  TwoWire();
  void begin();
  void begin(uint8_t);
  void begin(int);
  void end();
  void setClock(uint32_t);

  void setWireTimeout(uint32_t timeout = 25000, bool reset_with_timeout = false);
  bool getWireTimeoutFlag(void);
  void clearWireTimeoutFlag(void);

  void beginTransmission(uint8_t);
  void beginTransmission(int);

  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *, size_t);
  inline size_t write(unsigned long n) { return write((uint8_t)n); }
  inline size_t write(long n) { return write((uint8_t)n); }
  inline size_t write(unsigned int n) { return write((uint8_t)n); }
  inline size_t write(int n) { return write((uint8_t)n); }
  using Print::write;
  virtual void flush(void);

  uint8_t endTransmission(void);
  uint8_t endTransmission(uint8_t);

  uint8_t requestFrom(uint8_t, uint8_t);
  uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
  uint8_t requestFrom(int, int);
  uint8_t requestFrom(int, int, int);
  virtual int available(void);
  virtual int read(void);
  virtual int peek(void);

  void onReceive(void (*)(int));
  void onRequest(void (*)(void));
};

extern TwoWire Wire;

#endif
