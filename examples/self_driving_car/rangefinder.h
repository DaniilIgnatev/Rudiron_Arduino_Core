#pragma once

#include "Arduino.h"
#include "directions.h"

//API определения расстояния

void setup_rangefinder();

float measureDistance();

void scan_range(DirectionsEnum direction);

bool isObstacle(DirectionsEnum direction);
