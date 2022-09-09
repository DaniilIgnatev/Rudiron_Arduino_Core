#pragma once

#include "Arduino.h"
#include "directions.h"

//Миллисекунд поворота в одном градусе
#define DEGREE_MS 100

//Миллисекунд движения в одном метре
#define METER_MS 1000 

//API управление движением

void setup_driver();

void drive_towards(DirectionsEnum direction);

void driver_stop();
