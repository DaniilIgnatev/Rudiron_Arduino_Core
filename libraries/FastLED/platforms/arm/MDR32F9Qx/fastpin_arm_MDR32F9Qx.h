#ifndef __FASTPIN_ARM_MDR32F9Qx_H
#define __FASTPIN_ARM_MDR32F9Qx_H

FASTLED_NAMESPACE_BEGIN

// Заглушка
inline int delayNanoseconds(int){
    return 0;
}

// #if defined(FASTLED_FORCE_SOFTWARE_PINS)
// #warning "Software pin support forced, pin access will be slightly slower."
// #define NO_HARDWARE_PIN_SUPPORT
// #undef HAS_HARDWARE_PIN_SUPPORT

// #else

// #endif // FASTLED_FORCE_SOFTWARE_PINS

FASTLED_NAMESPACE_END

#endif // __INC_FASTPIN_ARM_STM32
