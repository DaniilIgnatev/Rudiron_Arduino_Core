/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
*/

#ifndef GPIO_H
#define GPIO_H

#include "config.h"
#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"

#include "gpio_utility.h"

namespace Rudiron
{
    class GPIO
    {
    public:
        static void enableCLK(PortName portName);

        static void disableCLK(PortName portName);

        static MDR_PORT_TypeDef *getPortPointer(PortPinName pinName);

        static PortName getPortName(PortPinName pinName);

        static void configPin(PortPinName name, PORT_InitTypeDef &config);

        static void configPinInput(PortPinName name);

        static void configPinInputPullUp(PortPinName name);

        static void configPinInputPullDown(PortPinName name);

        static void configPinOutput(PortPinName name);

        static void writeWord(PortName name, uint16_t value);

        static void writePin(PortPinName name, bool value);

        static uint16_t readWord(PortName name);

        static bool readPin(PortPinName name);

        static unsigned long pulseIn(PortPinName pin, bool state, unsigned long timeout);

        static PortPinName get_rudiron_gpio(uint8_t pin);

        static uint16_t get_arduino_gpio(PortPinName pinName);

#ifdef LED_BUILTIN_1
        static inline void writeLED_1(bool value)
        {
            GPIO::writePin(get_rudiron_gpio(LED_BUILTIN_1), value);
        }
#endif

#ifdef LED_BUILTIN_2
        static inline void writeLED_2(bool value)
        {
            GPIO::writePin(get_rudiron_gpio(LED_BUILTIN_2), value);
        }
#endif

#ifdef BUTTON_BUILTIN_1
        static inline bool readButton_1()
        {
            return GPIO::readPin(get_rudiron_gpio(BUTTON_BUILTIN_1));
        }
#endif

#ifdef BUTTON_BUILTIN_2
        static inline bool readButton_2()
        {
            return GPIO::readPin(get_rudiron_gpio(BUTTON_BUILTIN_2));
        }
#endif

#ifdef BUTTON_BUILTIN_3
        static inline bool readButton_3()
        {
            return GPIO::readPin(get_rudiron_gpio(BUTTON_BUILTIN_3));
        }
#endif
    };

}

#endif // GPIO_H
