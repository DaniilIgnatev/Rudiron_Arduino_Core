#pragma once

#include "Arduino.h"
#include "rudiron/tasks_timer.h"

///Таймер для отложенного вызова функций.
///Используйте для решения задач, менее требовательных к процессорному времени.
///Возможны, хотя и не желательны, блокировки и вызовы функций delay.
extern TasksTimer tasksTimer;

// Скетчи примеров

// #define NRF_TRANSMITTER
#include "nrf24_tank/tank_receiver_sketch.h"
#include "nrf24_tank/tank_transmitter_sketch.h"

#include "display_basic/display_sketch.h"