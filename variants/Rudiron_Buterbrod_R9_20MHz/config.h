/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron_MDR32F9Qx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron_MDR32F9Qx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron_MDR32F9Qx. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
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


#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include "pins_arduino.h"
#include "rudiron/can_types.h"

#ifdef HCLK_DISABLE
#define DEFAULT_RST_CLK_CPU_PLLmul RST_CLK_CPU_PLLmul2
#else
#define DEFAULT_RST_CLK_CPU_PLLmul RST_CLK_CPU_PLLmul1
#endif

// CLK
#define MICROS_STEP ((uint8_t)10)
extern volatile uint64_t _micros;

// Timer
extern void (*Timer_1_Interrupt_Handler)(void);
extern void (*Timer_2_Interrupt_Handler)(void);
extern void (*Timer_3_Interrupt_Handler)(void);

extern void (*Timer_1_DMA_Interrupt_Handler)(void);
extern void (*Timer_2_DMA_Interrupt_Handler)(void);
extern void (*Timer_3_DMA_Interrupt_Handler)(void);

// TASKS
#define TASKS_TIMER_ENABLED

// UART
/// Фактически на 1 байт меньше, чтобы не переполнить буфер
#define SERIAL_RX_BUFFER_LENGTH 64

typedef uint8_t UART_BUFFER_INDEX_T;

extern UART_BUFFER_INDEX_T _uart1_rx_buffer_head;
extern UART_BUFFER_INDEX_T _uart1_rx_buffer_tail;
extern uint8_t _uart1_rx_buffer[SERIAL_RX_BUFFER_LENGTH];

extern UART_BUFFER_INDEX_T _uart2_rx_buffer_head;
extern UART_BUFFER_INDEX_T _uart2_rx_buffer_tail;
extern uint8_t _uart2_rx_buffer[SERIAL_RX_BUFFER_LENGTH];

// #define CAN_RX_BUFFER_LENGTH 32
#ifndef CAN_RX_BUFFER_LENGTH
#define CAN_RX_BUFFER_LENGTH 0
#endif
extern Rudiron::CAN_RX_Package _can_rx_buffer[CAN_RX_BUFFER_LENGTH];

// NRF24
/// Чтение данных по прерыванию. Раскомментируйте здесь или добавьте в аргументы компилятора
// #define NRF24_USE_INTERRUPT
typedef uint8_t NRF24_BUFFER_INDEX_T;
extern NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_head;
extern NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_tail;

#define NRF24_PAYLOAD_LENGTH 32
/// Фактически на 1 байт меньше, чтобы не переполнить буфер
#define NRF24_RX_BUFFER_LENGTH (NRF24_PAYLOAD_LENGTH - 1) * 2
extern uint8_t _nrf24_rx_buffer[NRF24_RX_BUFFER_LENGTH];

// DAC
extern void (*DAC_DMA_Interrupt_Handler)(void);

// EXTERNAL INTERRUPTS
extern void (*interrupt_callback_EXT_INT1_IRQHandler)(void);
extern void (*interrupt_callback_EXT_INT2_IRQHandler)(void);
extern void (*interrupt_callback_EXT_INT3_IRQHandler)(void);
extern void (*interrupt_callback_EXT_INT4_IRQHandler)(void);

#endif