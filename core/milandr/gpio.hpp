/*
This file is part of DIBotQBS.

DIBotQBS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DIBotQBS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DIBotQBS.  If not, see <https://www.gnu.org/licenses/>.
*/



#ifndef PORT_H
#define PORT_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"


typedef enum {
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_F
} __attribute__ ((__packed__)) PortName;


typedef enum {
    PIN_NONE,
    PIN_A0,
    PIN_A1,
    PIN_A2,
    PIN_A3,
    PIN_A4,
    PIN_A5,
    PIN_A6,
    PIN_A7,

    PIN_B0,
    PIN_B1,
    PIN_B2,
    PIN_B3,
    PIN_B4,
    PIN_B5,
    PIN_B6,
    PIN_B7,
    PIN_B8,
    PIN_B9,
    PIN_B10,

    PIN_C0,
    PIN_C1,
    PIN_C2,

    PIN_D0,
    PIN_D1,
    PIN_D2,
    PIN_D3,
    PIN_D4,
    PIN_D5,
    PIN_D6,
    PIN_D7,

    PIN_E0,
    PIN_E1,
    PIN_E2,
    PIN_E3,
    PIN_E4,
    PIN_E5,
    PIN_E6,
    PIN_E7,

    PIN_F0,
    PIN_F1,
    PIN_F2,
    PIN_F3
} __attribute__ ((__packed__)) PortPinName;


class GPIO {
public:
#ifdef RUDIRON_BUTERBROD_rev3
    static const PortPinName LED_BUILTIN_1 = PIN_A4;
    static const PortPinName LED_BUILTIN_2 = PIN_A5;
    static const PortPinName BUTTON_BUILTIN_1 = PIN_A1;
    static const PortPinName BUTTON_BUILTIN_2 = PIN_A2;
    static const PortPinName BUTTON_BUILTIN_3 = PIN_A3;
#endif

#ifdef RUDIRON_BUTERBROD_rev4
    static const PortPinName LED_BUILTIN_1 = PIN_A2;
    static const PortPinName LED_BUILTIN_2 = PIN_A4;
    static const PortPinName BUTTON_BUILTIN_1 = PIN_A0;
    static const PortPinName BUTTON_BUILTIN_2 = PIN_B10;
#endif

#if defined RUDIRON_BUTERBROD_rev5
    const static PortPinName LED_BUILTIN_1 = PIN_A2;
    const static PortPinName LED_BUILTIN_2 = PIN_A4;
    const static PortPinName BUTTON_BUILTIN_1 = PIN_A0;
    const static PortPinName BUTTON_BUILTIN_2 = PIN_B10;
    const static PortPinName BUTTON_BUILTIN_3 = PIN_B9;
#endif

public:

    GPIO();

    static inline void writeLED_1(bool value) {
        GPIO::writePin(LED_BUILTIN_1, value);
    }

    static inline void writeLED_2(bool value) {
        GPIO::writePin(LED_BUILTIN_2, value);
    }

    static inline bool readButton_1() {
        return GPIO::readPin(BUTTON_BUILTIN_1);
    }

    static inline bool readButton_2() {
        return GPIO::readPin(BUTTON_BUILTIN_2);
    }

#if !defined RUDIRON_BUTERBROD_rev4

    static inline bool readButton_3() {
        return GPIO::readPin(BUTTON_BUILTIN_3);
    }

#endif


    static uint16_t getPinNumber(PortPinName pinName);


    static MDR_PORT_TypeDef *getPortPointer(PortPinName pinName);


    static PortName getPortName(PortPinName pibName);


    static void configPin(PortPinName name, PORT_InitTypeDef &config);


    static void configPinInput(PortPinName name);


    static void configPinInputPullUp(PortPinName name);


    static void configPinInputPullDown(PortPinName name);


    static void configPinOutput(PortPinName name);


    static void writeWord(PortName name, uint16_t value);


    static void writePin(PortPinName name, bool value);


    static uint16_t readWord(PortName name);


    static bool readPin(PortPinName name);

};

#endif // PORT_H
