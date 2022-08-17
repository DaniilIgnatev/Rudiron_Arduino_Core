#pragma once

#include "sketch.h"
#include "rudiron/nrf24.h"

#ifdef NRF_TRANSMITTER

extern bool pressed1;

extern bool pressed2;

extern bool pressed3;

void nrf24_tank_setup();

void nrf24_tank_loop();

#endif