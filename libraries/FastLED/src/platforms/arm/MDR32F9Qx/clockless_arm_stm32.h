#pragma once

#include "led_sysdefs.h"
#include "rudiron/gpio.h"

using namespace Rudiron;

#pragma GCC push_options
#pragma GCC optimize("O3")

#define BitIsSet(reg, bit) ((reg & (1 << bit)) != 0)

#define RES (uint8_t)11160

FASTLED_NAMESPACE_BEGIN
// Definition for a single channel clockless controller for the stm32 family of chips, like that used in the spark core
// See clockless.h for detailed info on how the template parameters are used.

#define FASTLED_HAS_CLOCKLESS 1

template <int DATA_PIN, int T1, int T2, int T3, EOrder RGB_ORDER = RGB, int XTRA0 = 0, bool FLIP = false, int WAIT_TIME = 50>
class ClocklessController : public CPixelLEDController<RGB_ORDER> {

public:
    virtual void init(){
        pinName = GPIO::get_rudiron_gpio(DATA_PIN);
        port = GPIO::getPortPointer(pinName);

        if (pinName >= PORT_PIN_A0 && pinName <= PORT_PIN_A7)
        {
            port_pin = 1 << (pinName - PORT_PIN_A0);
        }
        else if (pinName >= PORT_PIN_B0 && pinName <= PORT_PIN_B10)
        {
            port_pin = 1 << (pinName - PORT_PIN_B0);
        }
        else if (pinName >= PORT_PIN_C0 && pinName <= PORT_PIN_C2)
        {
            port_pin = 1 << (pinName - PORT_PIN_C0);
        }
        else if (pinName >= PORT_PIN_D0 && pinName <= PORT_PIN_D7)
        {
            port_pin = 1 << (pinName - PORT_PIN_D0);
        }
        else if (pinName >= PORT_PIN_E0 && pinName <= PORT_PIN_E7)
        {
            port_pin = 1 << (pinName - PORT_PIN_E0);
        }
        else if (pinName >= PORT_PIN_F0 && pinName <= PORT_PIN_F3)
        {
            port_pin = 1 << (pinName - PORT_PIN_F0);
        }

        PORT_InitTypeDef config;
        config.PORT_FUNC = ::PORT_FUNC_PORT;
        config.PORT_GFEN = ::PORT_GFEN_OFF;
        config.PORT_MODE = ::PORT_MODE_DIGITAL;
        config.PORT_OE = ::PORT_OE_OUT;
        config.PORT_PD = ::PORT_PD_DRIVER;
        config.PORT_PD_SHM = ::PORT_PD_SHM_OFF;
        config.PORT_PULL_DOWN = ::PORT_PULL_DOWN_OFF;
        config.PORT_PULL_UP = ::PORT_PULL_UP_OFF;
        config.PORT_SPEED = ::PORT_SPEED_MAXFAST;

        GPIO::configPin(pinName, config);
    }

    virtual uint16_t getMaxRefreshRate() const { return 400; }

protected:
    Rudiron::PortPinName pinName;
    MDR_PORT_TypeDef *port;
    uint32_t port_pin;
    uint32_t portdata_zero;
    uint32_t portdata_one;

    inline void transmit_zero()
    {
        volatile char b;

        port->RXTX = portdata_one;
        b = 0;
        b = 1;
        b = 2;
        b = 3;
        b = 4;
        b = 5;
        b = 6;
//        b = 7;
//        b = 8;
//        b = 9;

        port->RXTX = portdata_zero;

        b = 10;
        b = 11;
        b = 12;
        b = 13;
        b = 14;
        b = 15;
        b = 16;
        b = 17;

        b = 18;
        b = 19;
        b = 20;
        b = 21;
        b = 22;
//        b = 23;
//        b = 24;
//        b = 25;

        // b = 26;
        // b = 27;
        // b = 28;
        // b = 29;
        // b = 30;
        // b = 31;
        // b = 32;
        // b = 33;
    }

    inline void transmit_one()
    {
        volatile char b;

        port->RXTX = portdata_one;
        b = 0;
        b = 1;
        b = 2;
        b = 3;
        b = 4;
        b = 5;
        b = 6;
        b = 7;
        b = 8;
        b = 9;

        b = 10;
        b = 11;
//        b = 12;
//        b = 13;
//        b = 14;
//        b = 15;
//        b = 16;
//        b = 17;
//        b = 18;

        port->RXTX = portdata_zero;

        b = 19;
        b = 20;
        b = 21;
        b = 22;
        b = 23;
        b = 24;
        b = 25;
//        b = 26;
//        b = 27;
//        b = 28;
//
//        b = 29;
//        b = 30;
//        b = 31;
//        b = 32;
//        b = 33;
        // b = 34;
        // b = 35;
        // b = 36;
    }

    inline void transmit_byte(uint8_t data)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            if (BitIsSet(data, (7 - i)))
            {
                transmit_one();
            }
            else
            {
                transmit_zero();
            }
        }
    }

//    inline void transmit_reset()
//    {
//        GPIO::writePin(pinName, false);
//        for (size_t i = 0; i < 1000; i++)
//        {
//            asm volatile("mov r0, r0");
//        }
//    }

    virtual void showPixels(PixelController<RGB_ORDER> & pixels){
        uint32_t portdata = port->RXTX;
        portdata_one = portdata | port_pin;
        portdata_zero = portdata & (~port_pin);

        uint8_t b1, b2, b3;

        CLK::pause_delay();

        while (pixels.has(1))
        {
            pixels.stepDithering();

            b1 = pixels.loadAndScale0();
            transmit_byte(b1);

            b2 = pixels.loadAndScale1();
            transmit_byte(b2);

            b3 = pixels.loadAndScale2();
            transmit_byte(b3);

            pixels.advanceData();
        }

        _micros += (pixels.size() * 31.5 + 5);

        CLK::resume_delay();
//        transmit_reset();
    }
};

FASTLED_NAMESPACE_END

#pragma GCC pop_options