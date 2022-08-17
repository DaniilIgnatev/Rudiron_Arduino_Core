#include "tank_receiver_sketch.h"

#ifndef NRF_TRANSMITTER

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

void nrf24_tank_setup()
{
    if (!nrf24.begin(true))
    {
        Serial.println("Nrf24 Error!");
        return;
    }

    configEngines();
}

struct TankEngineModel
{
    bool isForwards = false;
    bool leftActive = false;
    bool rightActive = false;

    bool leftEngineFront = false;
    bool leftEngineBack = false;
    bool rightEngineFront = false;
    bool rightEngineBack = false;
};

TankEngineModel model;

const int testPackages_size = 8;

TankEngineModel truthTable[testPackages_size] = {
    TankEngineModel{true, false, false, false, false, false, false},
    TankEngineModel{true, true, false, true, false, false, true},
    TankEngineModel{true, false, true, false, true, true, false},
    TankEngineModel{true, true, true, true, false, true, false},
    TankEngineModel{false, false, false, false, false, false, false},
    TankEngineModel{false, true, false, false, true, true, false},
    TankEngineModel{false, false, true, true, false, false, true},
    TankEngineModel{false, true, true, false, true, false, true}};

void updateModelInputs()
{
    model.isForwards = nrf24.read();
    model.leftActive = nrf24.read();
    model.rightActive = nrf24.read();
}

void updateModelOutputs()
{
    for (auto engineTruthTable : truthTable)
    {
        if (engineTruthTable.isForwards == model.isForwards && engineTruthTable.leftActive == model.leftActive && engineTruthTable.rightActive == model.rightActive)
        {
            model = engineTruthTable;
            break;
        }
    }
}

void move()
{
    updateModelOutputs();

    digitalWrite(leftEngineFrontPin, model.leftEngineFront);
    digitalWrite(rightEngineFrontPin, model.rightEngineFront);
    digitalWrite(leftEngineBackPin, model.leftEngineBack);
    digitalWrite(rightEngineBackPin, model.rightEngineBack);

    digitalWrite(LED_BUILTIN_1, model.leftActive);
    digitalWrite(LED_BUILTIN_2, model.rightActive);
}

void nrf24_tank_loop()
{
    if (nrf24.available())
    {
        updateModelInputs();
        move();
    }

    delay(50);
}

#endif
