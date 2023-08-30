#ifndef GPIO_TYPES_H
#define GPIO_TYPES_H

namespace Rudiron
{
    typedef enum
    {
        PORT_A,
        PORT_B,
        PORT_C,
        PORT_D,
        PORT_E,
        PORT_F
    } PortName;

    typedef enum
    {
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
    } PortPinName;
}

#endif // GPIO_TYPES_H
