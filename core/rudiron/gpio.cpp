#include "gpio.h"

namespace Rudiron {

    GPIO::GPIO() {

    }


    uint16_t GPIO::getPinNumber(PortPinName pinName) {
        if (pinName >= PORT_PIN_A0 && pinName <= PORT_PIN_A7) {
            return 1 << (pinName - PORT_PIN_A0);
        } else if (pinName >= PORT_PIN_B0 && pinName <= PORT_PIN_B10) {
            return 1 << (pinName - PORT_PIN_B0);
        } else if (pinName >= PORT_PIN_C0 && pinName <= PORT_PIN_C2) {
            return 1 << (pinName - PORT_PIN_C0);
        } else if (pinName >= PORT_PIN_D0 && pinName <= PORT_PIN_D7) {
            return 1 << (pinName - PORT_PIN_D0);
        } else if (pinName >= PORT_PIN_E0 && pinName <= PORT_PIN_E7) {
            return 1 << (pinName - PORT_PIN_E0);
        } else if (pinName >= PORT_PIN_F0 && pinName <= PORT_PIN_F3) {
            return 1 << (pinName - PORT_PIN_F0);
        }

        return 0;
    }


    MDR_PORT_TypeDef *GPIO::getPortPointer(PortPinName pinName) {
        if (pinName >= PORT_PIN_A0 && pinName <= PORT_PIN_A7) {
            return MDR_PORTA;
        } else if (pinName >= PORT_PIN_B0 && pinName <= PORT_PIN_B10) {
            return MDR_PORTB;
        } else if (pinName >= PORT_PIN_C0 && pinName <= PORT_PIN_C2) {
            return MDR_PORTC;
        } else if (pinName >= PORT_PIN_D0 && pinName <= PORT_PIN_D7) {
            return MDR_PORTD;
        } else if (pinName >= PORT_PIN_E0 && pinName <= PORT_PIN_E7) {
            return MDR_PORTE;
        } else if (pinName >= PORT_PIN_F0 && pinName <= PORT_PIN_F3) {
            return MDR_PORTF;
        }

        return nullptr;
    }


    PortName GPIO::getPortName(PortPinName pinName) {
        if (pinName >= PORT_PIN_A0 && pinName <= PORT_PIN_A7) {
            return PORT_A;
        } else if (pinName >= PORT_PIN_B0 && pinName <= PORT_PIN_B10) {
            return PORT_B;
        } else if (pinName >= PORT_PIN_C0 && pinName <= PORT_PIN_C2) {
            return PORT_C;
        } else if (pinName >= PORT_PIN_D0 && pinName <= PORT_PIN_D7) {
            return PORT_D;
        } else if (pinName >= PORT_PIN_E0 && pinName <= PORT_PIN_E7) {
            return PORT_E;
        } else {
            return PORT_F;
        }
    }


    void GPIO::configPin(PortPinName pinName, PORT_InitTypeDef &config) {
        MDR_PORT_TypeDef *PORT = getPortPointer(pinName);
        uint32_t pin = getPinNumber(pinName);

        config.PORT_Pin = pin;
        PORT_Init(PORT, &config);
    }


    void GPIO::configPinInput(PortPinName name) {
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


    void GPIO::configPinInputPullUp(PortPinName name) {
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


    void GPIO::configPinInputPullDown(PortPinName name) {
        PORT_InitTypeDef config;
        config.PORT_FUNC = ::PORT_FUNC_PORT;
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


    void GPIO::configPinOutput(PortPinName name) {
        PORT_InitTypeDef config;
        config.PORT_FUNC = ::PORT_FUNC_PORT;
        config.PORT_GFEN = ::PORT_GFEN_OFF;
        config.PORT_MODE = ::PORT_MODE_DIGITAL;
        config.PORT_OE = ::PORT_OE_OUT;
        config.PORT_PD = ::PORT_PD_DRIVER;
        config.PORT_PD_SHM = ::PORT_PD_SHM_OFF;
        config.PORT_PULL_DOWN = ::PORT_PULL_DOWN_OFF;
        config.PORT_PULL_UP = ::PORT_PULL_UP_OFF;
        config.PORT_SPEED = ::PORT_SPEED_SLOW;

        configPin(name, config);
    }


    void GPIO::writeWord(PortName name, uint16_t value) {
        switch (name) {
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


    void GPIO::writePin(PortPinName name, bool value) {
        if (name >= PORT_PIN_A0 && name <= PORT_PIN_A7) {
            PORT_WriteBit(MDR_PORTA, 1 << (name - PORT_PIN_A0), (BitAction) value);
        } else if (name >= PORT_PIN_B0 && name <= PORT_PIN_B10) {
            PORT_WriteBit(MDR_PORTB, 1 << (name - PORT_PIN_B0), (BitAction) value);
        } else if (name >= PORT_PIN_C0 && name <= PORT_PIN_C2) {
            PORT_WriteBit(MDR_PORTC, 1 << (name - PORT_PIN_C0), (BitAction) value);
        } else if (name >= PORT_PIN_D0 && name <= PORT_PIN_D7) {
            PORT_WriteBit(MDR_PORTD, 1 << (name - PORT_PIN_D0), (BitAction) value);
        } else if (name >= PORT_PIN_E0 && name <= PORT_PIN_E7) {
            PORT_WriteBit(MDR_PORTE, 1 << (name - PORT_PIN_E0), (BitAction) value);
        } else if (name >= PORT_PIN_F0 && name <= PORT_PIN_F3) {
            PORT_WriteBit(MDR_PORTF, 1 << (name - PORT_PIN_F0), (BitAction) value);
        }
    }


    uint16_t GPIO::readWord(PortName name) {
        switch (name) {
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


    bool GPIO::readPin(PortPinName name) {
        if (name >= PORT_PIN_A0 && name <= PORT_PIN_A7) {
            return (bool) PORT_ReadInputDataBit(MDR_PORTA, 1 << (name - PORT_PIN_A0));
        } else if (name >= PORT_PIN_B0 && name <= PORT_PIN_B10) {
            return (bool) PORT_ReadInputDataBit(MDR_PORTB, 1 << (name - PORT_PIN_B0));
        } else if (name >= PORT_PIN_C0 && name <= PORT_PIN_C2) {
            return (bool) PORT_ReadInputDataBit(MDR_PORTC, 1 << (name - PORT_PIN_C0));
        } else if (name >= PORT_PIN_D0 && name <= PORT_PIN_D7) {
            return (bool) PORT_ReadInputDataBit(MDR_PORTD, 1 << (name - PORT_PIN_D0));
        } else if (name >= PORT_PIN_E0 && name <= PORT_PIN_E7) {
            return (bool) PORT_ReadInputDataBit(MDR_PORTE, 1 << (name - PORT_PIN_E0));
        } else if (name >= PORT_PIN_F0 && name <= PORT_PIN_F3) {
            return (bool) PORT_ReadInputDataBit(MDR_PORTF, 1 << (name - PORT_PIN_F0));
        }
        return false;
    }

}