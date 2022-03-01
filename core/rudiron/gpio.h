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



#ifndef GPIO_H
#define GPIO_H


#include "MDR_config.h"
#include "MDR32Fx.h"
#include "MDR_uart.h"
#include "MDR_port.h"
#include "MDR_rst_clk.h"

#include "pins_arduino.h"

namespace Rudiron {

    typedef enum {
        PORT_A,
        PORT_B,
        PORT_C,
        PORT_D,
        PORT_E,
        PORT_F
    } __attribute__ ((__packed__)) PortName;


    typedef enum {
        PORT_PIN_NONE,
        PORT_PIN_A0,
        PORT_PIN_A1,
        PORT_PIN_A2,
        PORT_PIN_A3,
        PORT_PIN_A4,
        PORT_PIN_A5,
        PORT_PIN_A6,
        PORT_PIN_A7,

        PORT_PIN_B0,
        PORT_PIN_B1,
        PORT_PIN_B2,
        PORT_PIN_B3,
        PORT_PIN_B4,
        PORT_PIN_B5,
        PORT_PIN_B6,
        PORT_PIN_B7,
        PORT_PIN_B8,
        PORT_PIN_B9,
        PORT_PIN_B10,

        PORT_PIN_C0,
        PORT_PIN_C1,
        PORT_PIN_C2,

        PORT_PIN_D0,
        PORT_PIN_D1,
        PORT_PIN_D2,
        PORT_PIN_D3,
        PORT_PIN_D4,
        PORT_PIN_D5,
        PORT_PIN_D6,
        PORT_PIN_D7,

        PORT_PIN_E0,
        PORT_PIN_E1,
        PORT_PIN_E2,
        PORT_PIN_E3,
        PORT_PIN_E4,
        PORT_PIN_E5,
        PORT_PIN_E6,
        PORT_PIN_E7,

        PORT_PIN_F0,
        PORT_PIN_F1,
        PORT_PIN_F2,
        PORT_PIN_F3
    } __attribute__ ((__packed__)) PortPinName;


    extern const PortPinName pinMap[NUM_DIGITAL_PINS];

    extern const PortPinName pinADCMap[NUM_ANALOG_INPUTS];


    class GPIO {
    public:

#ifdef LED_BUILTIN_1
        static inline void writeLED_1(bool value) {
            GPIO::writePin(LED_BUILTIN_1, value);
        }
#endif

#ifdef LED_BUILTIN_2
        static inline void writeLED_2(bool value) {
            GPIO::writePin(LED_BUILTIN_2, value);
        }
#endif

#ifdef BUTTON_BUILTIN_1
        static inline bool readButton_1() {
            return GPIO::readPin(BUTTON_BUILTIN_1);
        }
#endif

#ifdef BUTTON_BUILTIN_2
        static inline bool readButton_2() {
            return GPIO::readPin(BUTTON_BUILTIN_2);
        }
#endif

#ifdef BUTTON_BUILTIN_3
        static inline bool readButton_3() {
            return GPIO::readPin(BUTTON_BUILTIN_3);
        }
#endif

        static void enableCLK(PortName portName);


        static void disableCLK(PortName portName);


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

}

#endif // GPIO_H
