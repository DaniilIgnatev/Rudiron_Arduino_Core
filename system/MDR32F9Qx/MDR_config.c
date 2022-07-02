#include "MDR_config.h"


//CLK
__IO uint64_t _micros = 0;


//UART
UART_BUFFER_INDEX_T _uart1_rx_buffer_head = 0;
UART_BUFFER_INDEX_T _uart1_rx_buffer_tail = 0;
int _uart1_rx_buffer[SERIAL_RX_BUFFER_SIZE];

UART_BUFFER_INDEX_T _uart2_rx_buffer_head = 0;
UART_BUFFER_INDEX_T _uart2_rx_buffer_tail = 0;
int _uart2_rx_buffer[SERIAL_RX_BUFFER_SIZE];


//CAN
CAN_RX_Package _can_rx_buffer[CAN_RX_BUFFER_SIZE];


//NRF24
 NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_head = 0;
 NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_tail = 0;

 uint8_t _nrf24_rx_buffer[NRF24_RX_BUFFER_SIZE];
