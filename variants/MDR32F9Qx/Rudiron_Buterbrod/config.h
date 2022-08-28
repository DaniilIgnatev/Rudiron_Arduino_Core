#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#ifdef HCLK_DISABLE
#define DEFAULT_RST_CLK_CPU_PLLmul RST_CLK_CPU_PLLmul2 // 8mhz * 2
#else
#define DEFAULT_RST_CLK_CPU_PLLmul RST_CLK_CPU_PLLmul1 // 16mhz * 1
#endif

// CLK
#define MICROS_STEP ((uint8_t)10)
extern volatile uint64_t _micros;

// TASKS
#define TASKS_TIMER_ENABLED

// UART
///Фактически на 1 байт меньше, чтобы не переполнить буфер
#define SERIAL_RX_BUFFER_LENGTH 64

typedef uint8_t UART_BUFFER_INDEX_T;

extern UART_BUFFER_INDEX_T _uart1_rx_buffer_head;
extern UART_BUFFER_INDEX_T _uart1_rx_buffer_tail;
extern uint8_t _uart1_rx_buffer[SERIAL_RX_BUFFER_LENGTH];

extern UART_BUFFER_INDEX_T _uart2_rx_buffer_head;
extern UART_BUFFER_INDEX_T _uart2_rx_buffer_tail;
extern uint8_t _uart2_rx_buffer[SERIAL_RX_BUFFER_LENGTH];

// CAN
typedef struct
{
    uint32_t ID;
    uint8_t Data[8];
    uint8_t Mask;
} CAN_RX_Package;

#define CAN_RX_BUFFER_LENGTH 0
extern CAN_RX_Package _can_rx_buffer[CAN_RX_BUFFER_LENGTH];

// NRF24
// #define NRF24_USE_INTERRUPT
typedef uint8_t NRF24_BUFFER_INDEX_T;
extern NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_head;
extern NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_tail;

#define NRF24_PAYLOAD_LENGTH 32
///Фактически на 1 байт меньше, чтобы не переполнить буфер
#define NRF24_RX_BUFFER_LENGTH (NRF24_PAYLOAD_LENGTH - 1) * 2
extern uint8_t _nrf24_rx_buffer[NRF24_RX_BUFFER_LENGTH];

#endif