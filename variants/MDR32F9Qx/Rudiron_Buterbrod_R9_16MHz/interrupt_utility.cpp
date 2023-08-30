#include "interrupt_utility.h"
#include "rudiron/interrupt_types.h"
#include "rudiron/gpio.h"

#define EXT_INT1_NUMBER BUTTON_BUILTIN_1
#define EXT_INT2_NUMBER BUTTON_BUILTIN_2
#define EXT_INT4_NUMBER BUTTON_BUILTIN_3

int digitalPinToInterrupt(int pin)
{
    switch (pin)
    {
    case EXT_INT1_NUMBER:
        return (int)Rudiron::EXT_INT1_Interrupt;
    case EXT_INT2_NUMBER:
        return (int)Rudiron::EXT_INT2_Interrupt;
    case EXT_INT4_NUMBER:
        return (int)Rudiron::EXT_INT4_Interrupt;
    default:
        return (int)Rudiron::Interrupt_None;
    }
}