/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron_MDR32F9Qx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron_MDR32F9Qx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron_MDR32F9Qx. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
*/

#include "gpio.h"

namespace Rudiron
{
    void GPIO::enableCLK(PortName portName)
    {
        uint32_t PORT;

        switch (portName)
        {
        case PORT_A:
            PORT = RST_CLK_PCLK_PORTA;
            break;
        case PORT_B:
            PORT = RST_CLK_PCLK_PORTB;
            break;
        case PORT_C:
            PORT = RST_CLK_PCLK_PORTC;
            break;
        case PORT_D:
            PORT = RST_CLK_PCLK_PORTD;
            break;
        case PORT_E:
            PORT = RST_CLK_PCLK_PORTE;
            break;
        case PORT_F:
            PORT = RST_CLK_PCLK_PORTF;
            break;
        }
        RST_CLK_PCLKcmd(PORT, ENABLE);
    }

    void GPIO::disableCLK(PortName portName)
    {
        uint32_t PORT;

        switch (portName)
        {
        case PORT_A:
            PORT = RST_CLK_PCLK_PORTA;
            break;
        case PORT_B:
            PORT = RST_CLK_PCLK_PORTB;
            break;
        case PORT_C:
            PORT = RST_CLK_PCLK_PORTC;
            break;
        case PORT_D:
            PORT = RST_CLK_PCLK_PORTD;
            break;
        case PORT_E:
            PORT = RST_CLK_PCLK_PORTE;
            break;
        case PORT_F:
            PORT = RST_CLK_PCLK_PORTF;
            break;
        }
        RST_CLK_PCLKcmd(PORT, DISABLE);
    }

    MDR_PORT_TypeDef *GPIO::getPortPointer(PortPinName pinName)
    {
        if (pinName >= PORT_PIN_A0 && pinName <= PORT_PIN_A7)
        {
            return MDR_PORTA;
        }
        else if (pinName >= PORT_PIN_B0 && pinName <= PORT_PIN_B10)
        {
            return MDR_PORTB;
        }
        else if (pinName >= PORT_PIN_C0 && pinName <= PORT_PIN_C2)
        {
            return MDR_PORTC;
        }
        else if (pinName >= PORT_PIN_D0 && pinName <= PORT_PIN_D7)
        {
            return MDR_PORTD;
        }
        else if (pinName >= PORT_PIN_E0 && pinName <= PORT_PIN_E7)
        {
            return MDR_PORTE;
        }
        else if (pinName >= PORT_PIN_F0 && pinName <= PORT_PIN_F3)
        {
            return MDR_PORTF;
        }

        return nullptr;
    }

    PortName GPIO::getPortName(PortPinName pinName)
    {
        if (pinName >= PORT_PIN_A0 && pinName <= PORT_PIN_A7)
        {
            return PORT_A;
        }
        else if (pinName >= PORT_PIN_B0 && pinName <= PORT_PIN_B10)
        {
            return PORT_B;
        }
        else if (pinName >= PORT_PIN_C0 && pinName <= PORT_PIN_C2)
        {
            return PORT_C;
        }
        else if (pinName >= PORT_PIN_D0 && pinName <= PORT_PIN_D7)
        {
            return PORT_D;
        }
        else if (pinName >= PORT_PIN_E0 && pinName <= PORT_PIN_E7)
        {
            return PORT_E;
        }
        else
        {
            return PORT_F;
        }
    }

    void GPIO::configPin(PortPinName pinName, PORT_InitTypeDef &config)
    {
        PortName portName = getPortName(pinName);
        enableCLK(portName);

        MDR_PORT_TypeDef *PORT = getPortPointer(pinName);
        uint32_t pin = get_arduino_gpio(pinName);

        config.PORT_Pin = pin;
        PORT_Init(PORT, &config);
    }

    void GPIO::configPinInput(PortPinName name)
    {
        PORT_InitTypeDef config;
        config.PORT_FUNC = ::PORT_FUNC_PORT;
        config.PORT_GFEN = ::PORT_GFEN_OFF;
        config.PORT_MODE = ::PORT_MODE_DIGITAL;
        config.PORT_OE = ::PORT_OE_IN;
        config.PORT_PD = ::PORT_PD_DRIVER;
        config.PORT_PD_SHM = ::PORT_PD_SHM_OFF;
        config.PORT_PULL_DOWN = ::PORT_PULL_DOWN_OFF;
        config.PORT_PULL_UP = ::PORT_PULL_UP_OFF;
        config.PORT_SPEED = ::PORT_SPEED_SLOW;

        configPin(name, config);
    }

    void GPIO::configPinInputPullUp(PortPinName name)
    {
        PORT_InitTypeDef config;
        config.PORT_FUNC = ::PORT_FUNC_PORT;
        config.PORT_GFEN = ::PORT_GFEN_OFF;
        config.PORT_MODE = ::PORT_MODE_DIGITAL;
        config.PORT_OE = ::PORT_OE_IN;
        config.PORT_PD = ::PORT_PD_DRIVER;
        config.PORT_PD_SHM = ::PORT_PD_SHM_OFF;
        config.PORT_PULL_DOWN = ::PORT_PULL_DOWN_OFF;
        config.PORT_PULL_UP = ::PORT_PULL_UP_ON;
        config.PORT_SPEED = ::PORT_SPEED_SLOW;

        configPin(name, config);
    }

    void GPIO::configPinInputPullDown(PortPinName name)
    {
        PORT_InitTypeDef config;

        switch (name)
        {
        case PortPinName::PORT_PIN_A0:
            config.PORT_FUNC = ::PORT_FUNC_ALTER;
            break;
        case PortPinName::PORT_PIN_B10:
            config.PORT_FUNC = ::PORT_FUNC_ALTER;
            break;
        case PortPinName::PORT_PIN_B9:
            config.PORT_FUNC = ::PORT_FUNC_ALTER;
            break;
        default:
            config.PORT_FUNC = ::PORT_FUNC_PORT;
            break;
        }

        config.PORT_GFEN = ::PORT_GFEN_OFF;
        config.PORT_MODE = ::PORT_MODE_DIGITAL;
        config.PORT_OE = ::PORT_OE_IN;
        config.PORT_PD = ::PORT_PD_DRIVER;
        config.PORT_PD_SHM = ::PORT_PD_SHM_OFF;
        config.PORT_PULL_DOWN = ::PORT_PULL_DOWN_ON;
        config.PORT_PULL_UP = ::PORT_PULL_UP_OFF;
        config.PORT_SPEED = ::PORT_SPEED_SLOW;

        configPin(name, config);
    }

    void GPIO::configPinOutput(PortPinName name)
    {
        PORT_InitTypeDef config;
        config.PORT_FUNC = ::PORT_FUNC_PORT;
        config.PORT_GFEN = ::PORT_GFEN_OFF;
        config.PORT_MODE = ::PORT_MODE_DIGITAL;
        config.PORT_OE = ::PORT_OE_OUT;
        config.PORT_PD = ::PORT_PD_DRIVER;
        config.PORT_PD_SHM = ::PORT_PD_SHM_OFF;
        config.PORT_PULL_DOWN = ::PORT_PULL_DOWN_OFF;
        config.PORT_PULL_UP = ::PORT_PULL_UP_OFF;
        config.PORT_SPEED = ::PORT_SPEED_FAST;

        configPin(name, config);
    }

    void GPIO::writeWord(PortName name, uint16_t value)
    {
        switch (name)
        {
        case PortName::PORT_A:
            PORT_Write(MDR_PORTA, value);
            break;
        case PortName::PORT_B:
            PORT_Write(MDR_PORTB, value);
            break;
        case PortName::PORT_C:
            PORT_Write(MDR_PORTC, value);
            break;
        case PortName::PORT_D:
            PORT_Write(MDR_PORTD, value);
            break;
        case PortName::PORT_E:
            PORT_Write(MDR_PORTE, value);
            break;
        case PortName::PORT_F:
            PORT_Write(MDR_PORTF, value);
            break;
        }
    }

    void GPIO::writePin(PortPinName name, bool value)
    {
        if (name >= PORT_PIN_A0 && name <= PORT_PIN_A7)
        {
            PORT_WriteBit(MDR_PORTA, 1 << (name - PORT_PIN_A0), (BitAction)value);
        }
        else if (name >= PORT_PIN_B0 && name <= PORT_PIN_B10)
        {
            PORT_WriteBit(MDR_PORTB, 1 << (name - PORT_PIN_B0), (BitAction)value);
        }
        else if (name >= PORT_PIN_C0 && name <= PORT_PIN_C2)
        {
            PORT_WriteBit(MDR_PORTC, 1 << (name - PORT_PIN_C0), (BitAction)value);
        }
        else if (name >= PORT_PIN_D0 && name <= PORT_PIN_D7)
        {
            PORT_WriteBit(MDR_PORTD, 1 << (name - PORT_PIN_D0), (BitAction)value);
        }
        else if (name >= PORT_PIN_E0 && name <= PORT_PIN_E7)
        {
            PORT_WriteBit(MDR_PORTE, 1 << (name - PORT_PIN_E0), (BitAction)value);
        }
        else if (name >= PORT_PIN_F0 && name <= PORT_PIN_F3)
        {
            PORT_WriteBit(MDR_PORTF, 1 << (name - PORT_PIN_F0), (BitAction)value);
        }
    }

    uint16_t GPIO::readWord(PortName name)
    {
        switch (name)
        {
        case PortName::PORT_A:
            return PORT_ReadInputData(MDR_PORTA);
        case PortName::PORT_B:
            return PORT_ReadInputData(MDR_PORTB);
        case PortName::PORT_C:
            return PORT_ReadInputData(MDR_PORTC);
        case PortName::PORT_D:
            return PORT_ReadInputData(MDR_PORTD);
        case PortName::PORT_E:
            return PORT_ReadInputData(MDR_PORTE);
        case PortName::PORT_F:
            return PORT_ReadInputData(MDR_PORTF);
        }

        return 0;
    }

    bool GPIO::readPin(PortPinName name)
    {
        if (name >= PORT_PIN_A0 && name <= PORT_PIN_A7)
        {
            return (bool)PORT_ReadInputDataBit(MDR_PORTA, 1 << (name - PORT_PIN_A0));
        }
        else if (name >= PORT_PIN_B0 && name <= PORT_PIN_B10)
        {
            return (bool)PORT_ReadInputDataBit(MDR_PORTB, 1 << (name - PORT_PIN_B0));
        }
        else if (name >= PORT_PIN_C0 && name <= PORT_PIN_C2)
        {
            return (bool)PORT_ReadInputDataBit(MDR_PORTC, 1 << (name - PORT_PIN_C0));
        }
        else if (name >= PORT_PIN_D0 && name <= PORT_PIN_D7)
        {
            return (bool)PORT_ReadInputDataBit(MDR_PORTD, 1 << (name - PORT_PIN_D0));
        }
        else if (name >= PORT_PIN_E0 && name <= PORT_PIN_E7)
        {
            return (bool)PORT_ReadInputDataBit(MDR_PORTE, 1 << (name - PORT_PIN_E0));
        }
        else if (name >= PORT_PIN_F0 && name <= PORT_PIN_F3)
        {
            return (bool)PORT_ReadInputDataBit(MDR_PORTF, 1 << (name - PORT_PIN_F0));
        }
        return false;
    }

    unsigned long GPIO::pulseIn(PortPinName pin, bool state, unsigned long timeout)
    {
        unsigned long countdown_timeout = timeout;
        unsigned long countdown_started = countdown_timeout;
        bool started = false;

        while (countdown_timeout)
        {
            if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
            {
                countdown_timeout -= MICROS_STEP;
            }

            if (!started)
            {
                if (GPIO::readPin(pin) == state)
                {
                    started = true;
                    countdown_started = countdown_timeout;
                }
            }
            else
            {
                if (GPIO::readPin(pin) != state)
                {
                    break;
                }
            }
        }

        return countdown_started - countdown_timeout;
    }

    PortPinName __attribute__((optimize("O0"))) GPIO::get_rudiron_gpio(uint8_t pin)
    {
        return GPIOUtility::pinMap[pin];
    }

    uint16_t GPIO::get_arduino_gpio(PortPinName pinName)
    {
        if (pinName >= PORT_PIN_A0 && pinName <= PORT_PIN_A7)
        {
            return 1 << (pinName - PORT_PIN_A0);
        }
        else if (pinName >= PORT_PIN_B0 && pinName <= PORT_PIN_B10)
        {
            return 1 << (pinName - PORT_PIN_B0);
        }
        else if (pinName >= PORT_PIN_C0 && pinName <= PORT_PIN_C2)
        {
            return 1 << (pinName - PORT_PIN_C0);
        }
        else if (pinName >= PORT_PIN_D0 && pinName <= PORT_PIN_D7)
        {
            return 1 << (pinName - PORT_PIN_D0);
        }
        else if (pinName >= PORT_PIN_E0 && pinName <= PORT_PIN_E7)
        {
            return 1 << (pinName - PORT_PIN_E0);
        }
        else if (pinName >= PORT_PIN_F0 && pinName <= PORT_PIN_F3)
        {
            return 1 << (pinName - PORT_PIN_F0);
        }

        return 0;
    }
}