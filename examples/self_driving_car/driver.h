#pragma once

#include "Arduino.h"
#include "directions.h"

//Миллисекунд движения в одном метре
#define METER_MS 5000 

enum DriverSpeedEnum{
    stop,//полная остановка
    first,//при напряжении питания > 7В
    second,//при напряжении питания 6.5В
    third//при напряжении питания 6В 
};

//API управление движением

void setup_driver();

void drive_towards(DirectionsEnum direction, DriverSpeedEnum speed);

void drive_stop();