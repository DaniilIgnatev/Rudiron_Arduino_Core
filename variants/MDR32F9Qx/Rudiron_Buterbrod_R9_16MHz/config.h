#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include "pins_arduino.h"
#include "rudiron/can_types.h"

#ifdef HCLK_DISABLE
#define DEFAULT_RST_CLK_CPU_PLLmul RST_CLK_CPU_PLLmul2 // 8mhz * 2
#else
#define DEFAULT_RST_CLK_CPU_PLLmul RST_CLK_CPU_PLLmul1 // 16mhz * 1
#endif

// CLK
#define MICROS_STEP ((uint8_t)10)
extern volatile uint64_t _micros;

// Timer
extern void (*Timer_1_DMA_Interrupt_Handler)(void);

extern void (*Timer_2_DMA_Interrupt_Handler)(void);

extern void (*Timer_3_DMA_Interrupt_Handler)(void);

// TASKS
#define TASKS_TIMER_ENABLED

// I2C
extern void (*I2C_TX_Event)(void);
extern void (*I2C_RX_Event)(uint8_t *, int);

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
// #define NRF24_USE
// #define NRF24_USE_INTERRUPT
typedef uint8_t NRF24_BUFFER_INDEX_T;
extern NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_head;
extern NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_tail;

#define NRF24_PAYLOAD_LENGTH 32
/// Фактически на 1 байт меньше, чтобы не переполнить буфер
#define NRF24_RX_BUFFER_LENGTH (NRF24_PAYLOAD_LENGTH - 1) * 2
extern uint8_t _nrf24_rx_buffer[NRF24_RX_BUFFER_LENGTH];

extern void (*DAC_DMA_Interrupt_Handler)(void);

#endif