#pragma once

// Include the sam headers
//#include "fastpin_arm_stm32.h"
// #include "fastspi_arm_stm32.h"
#include "clockless_arm_stm32.h"
// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

