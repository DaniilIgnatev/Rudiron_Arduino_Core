#pragma once

#include "sketch.h"
#include <array>

#ifndef TRANSMITTER

struct TankReceiverModel {
    bool isForwards = false;
    bool leftActive = false;
    bool rightActive = false;

    bool leftEngineFront = false;
    bool leftEngineBack = false;
    bool rightEngineFront = false;
    bool rightEngineBack = false;

    bool operator==(const TankReceiverModel &rhs) {
        return this->isForwards == rhs.isForwards &&
               this->leftActive == rhs.leftActive &&
               this->rightActive == rhs.rightActive &&

               this->leftEngineFront == rhs.leftEngineFront &&
               this->leftEngineBack == rhs.leftEngineBack &&
               this->rightEngineFront == rhs.rightEngineFront &&
               this->rightEngineBack == rhs.rightEngineBack;
    }
};

    TankReceiverModel getModel(bool isForwards, bool leftActive, bool rightActive);

    bool hasNextTestDataPackage();

    TankReceiverModel nextTestDataPackage();

void tank_setup();

void tank_loop();

#endif