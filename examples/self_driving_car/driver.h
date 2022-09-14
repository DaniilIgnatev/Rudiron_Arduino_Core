#pragma once

#include "Arduino.h"
#include "directions.h"

//Миллисекунд движения в одном метре
#define METER_MS 5000 

enum DriverSpeedEnum{
    stop,
    first,
    second,
    third
};

//API управление движением

void setup_driver();

void drive_towards(DirectionsEnum direction, DriverSpeedEnum speed);

void drive_stop();