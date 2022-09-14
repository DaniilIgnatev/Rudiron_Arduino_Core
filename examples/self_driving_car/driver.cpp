#include "driver.h"

#define leftEngineDIR 16
#define leftEngineSPEED 12
#define rightEngineDIR 21
#define rightEngineSPEED 20

void setup_driver()
{
    pinMode(leftEngineDIR, OUTPUT);
    pinMode(leftEngineSPEED, OUTPUT);
    pinMode(rightEngineDIR, OUTPUT);
    pinMode(rightEngineSPEED, OUTPUT);
}

struct DriverModel
{
public:
    uint8_t speed = 0;

    bool leftDir = false;
    bool rightDir = false;
};

uint8_t getSpeedValue(DriverSpeedEnum speed)
{
    switch (speed)
    {
    case DriverSpeedEnum::stop:
        return 0;
    case DriverSpeedEnum::first:
        return 85;
    case DriverSpeedEnum::second:
        return 170;
    case DriverSpeedEnum::third:
        return 255;
    }
}

DriverModel getModel(DirectionsEnum direction, uint8_t speed)
{
    DriverModel driver_model;
    driver_model.speed = speed;

    switch (direction)
    {
    case DirectionsEnum::straight:
        driver_model.leftDir = true;
        driver_model.rightDir = true;
        break;
    case DirectionsEnum::backwards:
        driver_model.leftDir = false;
        driver_model.rightDir = false;
        break;
    case DirectionsEnum::left:
        driver_model.leftDir = false;
        driver_model.rightDir = true;
        break;
    case DirectionsEnum::right:
        driver_model.leftDir = true;
        driver_model.rightDir = false;
        break;
    }

    return driver_model;
}

void applyModel(DriverModel model)
{
    digitalWrite(leftEngineDIR, model.leftDir);
    // digitalWrite(leftEngineSPEED, model.speed);
    analogWrite(leftEngineSPEED, model.speed);

    digitalWrite(rightEngineDIR, model.rightDir);
    // digitalWrite(rightEngineSPEED, model.speed);
    analogWrite(rightEngineSPEED, model.speed);
}

void drive_towards(DirectionsEnum direction, DriverSpeedEnum speed)
{
    uint8_t speedValue = getSpeedValue(speed);
    DriverModel model = getModel(direction, speedValue);
    applyModel(model);
}

void drive_stop(){
    drive_towards(DirectionsEnum::straight, DriverSpeedEnum::stop);
}