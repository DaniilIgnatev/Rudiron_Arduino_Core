/*
This file is part of Arduino_Core_Rudiron.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron.  If not, see <https://www.gnu.org/licenses/>.
*/



#ifndef ARDUINO_CORE_RUDIRON_PINS_ARDUINO_H
#define ARDUINO_CORE_RUDIRON_PINS_ARDUINO_H

#include <stdint.h>
#include "gpio_map.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_DIGITAL_PINS            36
#define NUM_ANALOG_INPUTS           6

/* Default for Arduino connector compatibility */
/* SPI Definitions */
#ifndef PIN_SPI_SS
#define PIN_SPI_SS                10
#endif
#ifndef PIN_SPI_SS1
#define PIN_SPI_SS1               4
#endif
#ifndef PIN_SPI_SS2
#define PIN_SPI_SS2               7
#endif
#ifndef PIN_SPI_SS3
#define PIN_SPI_SS3               8
#endif
#ifndef PIN_SPI_MOSI
#define PIN_SPI_MOSI              11
#endif
#ifndef PIN_SPI_MISO
#define PIN_SPI_MISO              12
#endif
#ifndef PIN_SPI_SCK
#define PIN_SPI_SCK               13
#endif

static const uint32_t SS   = PIN_SPI_SS;
static const uint32_t SS1  = PIN_SPI_SS1;
static const uint32_t SS2  = PIN_SPI_SS2;
static const uint32_t SS3  = PIN_SPI_SS3;
static const uint32_t MOSI = PIN_SPI_MOSI;
static const uint32_t MISO = PIN_SPI_MISO;
static const uint32_t SCK  = PIN_SPI_SCK;

/* I2C Definitions */
#ifndef PIN_WIRE_SDA
#define PIN_WIRE_SDA              14
#endif
#ifndef PIN_WIRE_SCL
#define PIN_WIRE_SCL              15
#endif

static const uint32_t SDA = PIN_WIRE_SDA;
static const uint32_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 4

#define PIN_A0   (54)
#define PIN_A1   (55)
#define PIN_A2   (56)
#define PIN_A3   (57)
#define PIN_A4   (58)
#define PIN_A5   (59)



static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;




// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE2       Serial2
#define SERIAL_PORT_HARDWARE3       Serial3
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial2
#define SERIAL_PORT_HARDWARE_OPEN2  Serial3

#ifdef __cplusplus
}
#endif

#endif //ARDUINO_CORE_RUDIRON_PINS_ARDUINO_H
