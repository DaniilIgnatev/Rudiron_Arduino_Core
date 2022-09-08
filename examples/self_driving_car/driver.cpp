#include "driver.h"

#define leftEngineFrontPin 12
#define leftEngineBackPin 13
#define rightEngineFrontPin 14
#define rightEngineBackPin 15

void setup_driver()
{
    pinMode(leftEngineFrontPin, OUTPUT);
    pinMode(leftEngineBackPin, OUTPUT);
    pinMode(rightEngineFrontPin, OUTPUT);
    pinMode(rightEngineBackPin, OUTPUT);
}

struct DriverModel
{
public:
    bool isForwards = false;
    bool leftActive = false;
    bool rightActive = false;

    bool leftEngineFront = false;
    bool leftEngineBack = false;
    bool rightEngineFront = false;
    bool rightEngineBack = false;
};

DriverModel driver_model;

const int model_table_size = 8;

DriverModel modelTable[model_table_size] = {
    DriverModel{true, false, false, false, false, false, false},
    DriverModel{true, true, false, true, false, false, true},
    DriverModel{true, false, true, false, true, true, false},
    DriverModel{true, true, true, true, false, true, false},
    DriverModel{false, false, false, false, false, false, false},
    DriverModel{false, true, false, false, true, true, false},
    DriverModel{false, false, true, true, false, false, true},
    DriverModel{false, true, true, false, true, false, true}};

void drive(bool isForwards, bool leftActive, bool rightActive)
{
    driver_model.isForwards = isForwards;
    driver_model.leftActive = leftActive;
    driver_model.rightActive = rightActive;

    for (auto matchedModel : modelTable)
    {
        if (matchedModel.isForwards == driver_model.isForwards && matchedModel.leftActive == driver_model.leftActive && matchedModel.rightActive == driver_model.rightActive)
        {
            driver_model = matchedModel;
            break;
        }
    }
}

void drive(DirectionsEnum direction)
{
    bool leftActive = direction == DirectionsEnum::left || direction == DirectionsEnum::straight;
    bool rightActive = direction == DirectionsEnum::right || direction == DirectionsEnum::straight;

    drive(true, leftActive, rightActive);
}