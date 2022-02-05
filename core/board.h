#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Core and peripherals registers definitions
*/
//#include "interrupt.h"
//#include "analog.h"
//#include "backup.h"
//#include "clock.h"
//#include "core_callback.h"
//#include "digital_io.h"
//#include "dwt.h"
//#include "hw_config.h"
//#include "otp.h"
//#include "timer.h"
//#include "uart.h"

#include "milandr/adc.hpp"
#include "milandr/can.hpp"
#include "milandr/can.hpp"
#include "milandr/clk.hpp"
#include "milandr/clk.hpp"
#include "milandr/gpio.h"
#include "milandr/gpio.h"
#include "milandr/spi.hpp"
#include "milandr/spi.hpp"
#include "milandr/timer.hpp"
#include "milandr/timer.hpp"
#include "milandr/uart.hpp"
#include "milandr/uart.hpp"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void init(void) ;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _BOARD_H_ */
