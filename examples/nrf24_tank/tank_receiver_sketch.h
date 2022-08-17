#pragma once

#include "sketch.h"
#include "rudiron/nrf24.h"

#ifndef NRF_TRANSMITTER

void nrf24_tank_setup();

void nrf24_tank_loop();

#endif