#include "tank_receiver.h"

#ifndef TRANSMITTER

#define leftEngineFrontPin 12
#define leftEngineBackPin 13
#define rightEngineFrontPin 14
#define rightEngineBackPin 15

using namespace Rudiron;

void configEngines()
{
    pinMode(leftEngineFrontPin, OUTPUT);
    pinMode(leftEngineBackPin, OUTPUT);
    pinMode(rightEngineFrontPin, OUTPUT);
    pinMode(rightEngineBackPin, OUTPUT);
}

void tank_setup()
{
    if (!nrf24.begin(true))
    {
        Serial.println("Nrf24 Error!");
        return;
    }

    configEngines();
}

const int testPackages_size = 8;

int testPackages_index = 0;

std::array<TankReceiverModel, testPackages_size> truthTable = {
    TankReceiverModel{true, false, false, false, false, false, false},
    TankReceiverModel{true, true, false, true, false, false, true},
    TankReceiverModel{true, false, true, false, true, true, false},
    TankReceiverModel{true, true, true, true, false, true, false},
    TankReceiverModel{false, false, false, false, false, false, false},
    TankReceiverModel{false, true, false, false, true, true, false},
    TankReceiverModel{false, false, true, true, false, false, true},
    TankReceiverModel{false, true, true, false, true, false, true}};

TankReceiverModel updateModel()
{
    TankReceiverModel result;

    for (auto truthModel : truthTable)
    {
        if (truthModel.isForwards == model.isForwards && truthModel.leftActive == model.leftActive && truthModel.rightActive == model.rightActive)
        {
            result = truthModel;
            break;
        }
    }

    return result;
}

bool hasNextTestDataPackage()
{
    return testPackages_index != testPackages_size;
}

TankReceiverModel nextTestDataPackage()
{
    return truthTable[testPackages_index++];
}

TankReceiverModel model;

void move()
{
    model = updateModel();

    digitalWrite(leftEngineFrontPin, model.leftEngineFront);
    digitalWrite(rightEngineFrontPin, model.rightEngineFront);
    digitalWrite(leftEngineBackPin, model.leftEngineBack);
    digitalWrite(rightEngineBackPin, model.rightEngineBack);

    digitalWrite(LED_BUILTIN_1, model.leftActive);
    digitalWrite(LED_BUILTIN_2, model.rightActive);
}

void tank_loop()
{
    if (nrf24.available())
    {
        model.isForwards = nrf24.read();
        model.leftActive = nrf24.read();
        model.rightActive = nrf24.read();

        move();
    }

    delay(50);
}

#endif
