#ifndef __INC_LED_SYSDEFS_H
#define __INC_LED_SYSDEFS_H

#include "fastled_config.h"

/// @file led_sysdefs.h
/// Determines which platform system definitions to include

#include "platforms/arm/MDR32F9Qx/led_sysdefs_arm_stm32.h"

#define FASTLED_NAMESPACE_BEGIN
#define FASTLED_NAMESPACE_END
#define FASTLED_USING_NAMESPACE

// Arduino.h needed for convenience functions digitalPinToPort/BitMask/portOutputRegister and the pinMode methods.
#ifdef ARDUINO
#include <Arduino.h>
#endif

/// Clock cycles per microsecond. 
/// Calculated using the F_CPU preprocessor define
#define CLKS_PER_US (F_CPU/1000000)

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif
#endif
