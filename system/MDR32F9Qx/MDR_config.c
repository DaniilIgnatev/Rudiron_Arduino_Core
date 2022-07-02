#include "MDR_config.h"


//CLK
__IO uint64_t _micros = 0;


//UART
//указывает на следуюую ячеку для записи
UART_BUFFER_INDEX_T _uart1_rx_buffer_head = 0;
//указывает на следуюую ячеку для чтения
UART_BUFFER_INDEX_T _uart1_rx_buffer_tail = 0;
short _uart1_rx_buffer[SERIAL_RX_BUFFER_LENGTH];

//указывает на следуюую ячеку для записи
UART_BUFFER_INDEX_T _uart2_rx_buffer_head = 0;
//указывает на следуюую ячеку для чтения
UART_BUFFER_INDEX_T _uart2_rx_buffer_tail = 0;
short _uart2_rx_buffer[SERIAL_RX_BUFFER_LENGTH];


//CAN
CAN_RX_Package _can_rx_buffer[CAN_RX_BUFFER_LENGTH];


//NRF24
//указывает на следуюую ячеку для записи
 NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_head = 0;
 //указывает на следуюую ячеку для чтения
 NRF24_BUFFER_INDEX_T _nrf24_rx_buffer_tail = 0;

 short _nrf24_rx_buffer[NRF24_RX_BUFFER_LENGTH];
