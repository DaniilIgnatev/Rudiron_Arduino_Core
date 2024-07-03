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

#include "config.h"

// CLK
volatile uint64_t _micros = 0;

// Timer
void (*Timer_1_DMA_Interrupt_Handler)(void) = nullptr;
void (*Timer_2_DMA_Interrupt_Handler)(void) = nullptr;
void (*Timer_3_DMA_Interrupt_Handler)(void) = nullptr;

// UART
// указывает на следуюую ячеку для записи
UART_BUFFER_INDEX_T _uart1_rx_buffer_head = 0;
// указывает на следуюую ячеку для чтения
UART_BUFFER_INDEX_T _uart1_rx_buffer_tail = 0;
uint8_t _uart1_rx_buffer[SERIAL_RX_BUFFER_LENGTH];

// указывает на следуюую ячеку для записи
UART_BUFFER_INDEX_T _uart2_rx_buffer_head = 0;
// указывает на следуюую ячеку для чтения
UART_BUFFER_INDEX_T _uart2_rx_buffer_tail = 0;
uint8_t _uart2_rx_buffer[SERIAL_RX_BUFFER_LENGTH];

// CAN
Rudiron::CAN_RX_Package _can_rx_buffer[CAN_RX_BUFFER_LENGTH];

// NRF24
// указывает на следуюую ячеку для записи
NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_head = 0;
// указывает на следуюую ячеку для чтения
NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_tail = 0;

uint8_t _nrf24_rx_buffer[NRF24_RX_BUFFER_LENGTH];

// DAC
void (*DAC_DMA_Interrupt_Handler)(void) = nullptr;

// EXTERNAL INTERRUPTS
void (*interrupt_callback_EXT_INT1_IRQHandler)(void) = nullptr;
void (*interrupt_callback_EXT_INT2_IRQHandler)(void) = nullptr;
void (*interrupt_callback_EXT_INT3_IRQHandler)(void) = nullptr;
void (*interrupt_callback_EXT_INT4_IRQHandler)(void) = nullptr;
