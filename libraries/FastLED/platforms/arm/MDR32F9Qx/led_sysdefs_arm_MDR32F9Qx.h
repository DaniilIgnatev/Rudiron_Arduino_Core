#ifndef __INC_LED_SYSDEFS_ARM_MDR32F9Qx_H
#define __INC_LED_SYSDEFS_ARM_MDR32F9Qx_H

#define FASTLED_ARM

#ifndef INTERRUPT_THRESHOLD
#define INTERRUPT_THRESHOLD 1
#endif

// Default to allowing interrupts
#ifndef FASTLED_ALLOW_INTERRUPTS
#define FASTLED_ALLOW_INTERRUPTS 0
#endif

#if FASTLED_ALLOW_INTERRUPTS == 1
#define FASTLED_ACCURATE_CLOCK
#endif

// pgmspace definitions
#define PROGMEM

// The stm32duino core already defines these
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_dword_near(addr) pgm_read_dword(addr)

// Default to NOT using PROGMEM here
#ifndef FASTLED_USE_PROGMEM
#define FASTLED_USE_PROGMEM 0
#endif

// data type defs
typedef volatile uint8_t RoReg; /**< Read only 8-bit register (volatile const unsigned int) */
typedef volatile uint8_t RwReg; /**< Read-Write 8-bit register (volatile unsigned int) */

#define FASTLED_NO_PINMAP

#if defined(MDR32F9Qx)
#undef F_CPU
#define F_CPU 64000000
#define FASTLED_TEENSY4
#endif

#endif // defined(STM32F10X_MD) || defined(STM32F2XX)
