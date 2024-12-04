/**
  ******************************************************************************
  * @file    System_MDR32F9Qx.h
  * @author  Milandr Application Team
  * @version V1.2.0
  * @date    04/07/2011
  * @brief   This file contains all the functions prototypes for the interrupt.
  *
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Milandr</center></h2>
  */

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_MDR32F9Qx_H
#define __SYSTEM_MDR32F9Qx_H

#ifdef __cplusplus
extern "C" {
#endif

#include "inttypes.h"

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);


#ifdef __cplusplus
}
#endif

#endif
