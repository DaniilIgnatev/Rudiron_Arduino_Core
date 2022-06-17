#include "MDR_config.h"


//CLK
__IO uint64_t _micros = 0;


//UART
rx_buffer_index_t _uart1_rx_buffer_head = 0;
rx_buffer_index_t _uart1_rx_buffer_tail = 0;
// tx_buffer_index_t _uart1_tx_buffer_head = 0;
// tx_buffer_index_t _uart1_tx_buffer_tail = 0;
int _uart1_rx_buffer[SERIAL_RX_BUFFER_SIZE] = {0};
// int _uart1_tx_buffer[SERIAL_TX_BUFFER_SIZE] = {0};
rx_buffer_index_t _uart2_rx_buffer_head = 0;
rx_buffer_index_t _uart2_rx_buffer_tail = 0;
// tx_buffer_index_t _uart2_tx_buffer_head = 0;
// tx_buffer_index_t _uart2_tx_buffer_tail = 0;
int _uart2_rx_buffer[SERIAL_RX_BUFFER_SIZE] = {0};
// int _uart2_tx_buffer[SERIAL_TX_BUFFER_SIZE] = {0};


//CAN
CAN_RX_Package _can_rx_buffer[CAN_RX_BUFFER_SIZE];
