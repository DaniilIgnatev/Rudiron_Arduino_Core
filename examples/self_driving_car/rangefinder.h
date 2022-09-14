#pragma once

#include "Arduino.h"
#include "directions.h"

//Миллисекунд поворота в одном градусе
#define DEGREE_MS 5

//API определения расстояния

void setup_rangefinder();

void turnHead(DirectionsEnum direction);

float measureDistance();

void scan_range(DirectionsEnum direction);

bool isObstacle(DirectionsEnum direction);
