#pragma once

#include "Arduino.h"
#include "directions.h"

//API управление движением

void setup_driver();

void update_driver(DirectionsEnum direction);
