#pragma once

#include "Arduino.h"
#include "directions.h"

void setup_distance();

void scan_distance(DirectionsEnum direction);

bool isObstacle(DirectionsEnum direction);
