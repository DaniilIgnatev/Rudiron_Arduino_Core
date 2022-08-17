#pragma once

#include "Arduino.h"
#include "arduino-timer.h"

///Таймер миллисекундного разрешения для обработки до 16-х, не требующих высокой точности, задач.
///Используйте для задач, менее требовательных к процессорному времени.
///Возможны, хотя и не желательны, блокировки и вызовы функций delay.
extern Timer<> timer_lowPriority;

///Таймер микросекундного разрешения для обработки до 4-х высокоприоритетных задач.
///Используйте исключительно для задач, требующих минимальное процессорное время.
///Ни в коем случае не блокировать и не вызывать в задачах функции delay.
extern Timer<4, micros, void*> timer_highPriority;

// EXAMPLES

// #define NRF_TRANSMITTER
#include "nrf24_tank/tank_receiver_sketch.h"
#include "nrf24_tank/tank_transmitter_sketch.h"

#include "display_basic/display_sketch.h"