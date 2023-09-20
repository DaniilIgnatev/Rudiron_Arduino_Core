/**
  * FILE system_MDR32F9Qx.c
  */


/** @addtogroup __CMSIS CMSIS
  * @{
  */

/** @defgroup MDR1986VE9x
 *  @{
 */

/** @addtogroup __MDR32F9QX MDR32F9QX System
  * @{
  */

/** @addtogroup System_Private_Includes System Private Includes
  * @{
  */

#include "system_MDR32F9Qx.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"


void __initialize_data (unsigned int* from, unsigned int* region_begin, unsigned int* region_end);
void __initialize_bss (unsigned int* region_begin, unsigned int* region_end);
void __run_init_array (void);
void __run_fini_array (void);
void __initialize_hardware_early (void);
void __initialize_hardware (void);




inline __attribute__((always_inline)) void __initialize_data (unsigned int* from, unsigned int* region_begin,
                                                              unsigned int* region_end)
{
    // Iterate and copy word by word.
    // It is assumed that the pointers are word aligned.
    unsigned int *p = region_begin;
    while (p < region_end)
        *p++ = *from++;
}

inline __attribute__((always_inline)) void __initialize_bss (unsigned int* region_begin, unsigned int* region_end)
{
    // Iterate and clear word by word.
    // It is assumed that the pointers are word aligned.
    unsigned int *p = region_begin;
    while (p < region_end)
        *p++ = 0;
}

// These magic symbols are provided by the linker.
extern void (*__preinit_array_start[]) (void) __attribute__((weak));
extern void (*__preinit_array_end[]) (void) __attribute__((weak));
extern void (*__init_array_start[]) (void) __attribute__((weak));
extern void (*__init_array_end[]) (void) __attribute__((weak));
extern void (*__fini_array_start[]) (void) __attribute__((weak));
extern void (*__fini_array_end[]) (void) __attribute__((weak));

// Iterate over all the preinit/init routines (mainly static constructors).
inline __attribute__((always_inline)) void __run_init_array (void)
{
    int count;
    int i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
        __preinit_array_start[i]();

    // If you need to run the code in the .init section, please use
    // the startup files, since this requires the code in crti.o and crtn.o
    // to add the function prologue/epilogue.
    //_init(); // DO NOT ENABE THIS!

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i]();
}

// Run all the cleanup routines (mainly static destructors).
inline __attribute__((always_inline)) void __run_fini_array (void)
{
    int count;
    int i;

    count = __fini_array_end - __fini_array_start;
    for (i = count; i > 0; i--)
        __fini_array_start[i - 1]();

    // If you need to run the code in the .fini section, please use
    // the startup files, since this requires the code in crti.o and crtn.o
    // to add the function prologue/epilogue.
    //_fini(); // DO NOT ENABE THIS!
}


/**
  * @brief  Setup the microcontroller system
  *         RST clock configuration to the default reset state
  *         Setup SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
    // Call the standard library initialisation (mandatory for C++ to
    // execute the constructors for the static objects).
    __run_init_array();

    // Run the C++ static destructors.
    __run_fini_array();

#ifndef MDR_NO_RST_VTOR
    SCB->VTOR = 0x08000000;
#endif
}

/** @} */ /* End of group __MDR32F9QX_System_Private_Functions */

/** @} */ /* End of group __MDR32F9QX */

/** @} */ /* End of group MDR1986VE9x */

/** @} */ /* End of group __CMSIS */

/*
*
* END OF FILE system_MDR32F9Qx.c */
