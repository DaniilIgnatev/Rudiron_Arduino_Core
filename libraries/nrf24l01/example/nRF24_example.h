/*
This file is part of Arduino_Core_Rudiron.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron.  If not, see <https://www.gnu.org/licenses/>.
*/



#ifndef __NRF24_HAL_H
#define __NRF24_HAL_H

#ifdef __cplusplus
extern "C" {
#endif


// Hardware abstraction layer for NRF24L01+ transceiver (hardware depended functions)
// GPIO pins definition
// GPIO pins initialization and control functions
// SPI transmit functions


// Peripheral libraries
#include "../nrf24.h"


typedef enum {
    nRF24_DEMO_RX_SINGLE_ESB_MODE,
    nRF24_DEMO_TX_SINGLE_ESB_MODE,
    nRF24_DEMO_RX_SINGLE_MODE,
    nRF24_DEMO_RX_MULTI_MODE,
    nRF24_DEMO_RX_SOLAR_MODE,
    nRF24_DEMO_TX_SINGLE_MODE,
    nRF24_DEMO_TX_MULTI_MODE
} nRF24_DEMO_MODE;



// Function prototypes


void nRF24_DEMO_RX_SINGLE_ESB();


void nRF24_DEMO_TX_SINGLE_ESB();


void nRF24_DEMO_RX_SINGLE();


void nRF24_DEMO_RX_MULTI();


void nRF24_DEMO_RX_SOLAR();


void nRF24_DEMO_TX_SINGLE();


void nRF24_DEMO_TX_MULTI();

#ifdef __cplusplus
}
#endif

#endif // __NRF24_HAL_H
