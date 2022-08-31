#include "sketch.h"

#define IN_LEFT 12
#define PWM_LEFT 13
#define IN_RIGHT 14
#define PWM_RIGHT 15

using namespace Rudiron;

void configEngines()
{
    pinMode(IN_LEFT, OUTPUT);
    pinMode(PWM_LEFT, OUTPUT);
    pinMode(IN_RIGHT, OUTPUT);
    pinMode(PWM_RIGHT, OUTPUT);
}

void setup()
{
    if (!nrf24.begin(true))
    {
        Serial.println("Nrf24 Error!");
        return;
    }

    configEngines();
}

struct TankControlModel
{
    bool isForwards = false;
    bool leftActive = false;
    bool rightActive = false;
};

TankControlModel model;

void updateModelInputs()
{
    model.isForwards = nrf24.read();
    model.leftActive = nrf24.read();
    model.rightActive = nrf24.read();
}

void move()
{
    digitalWrite(IN_LEFT, model.isForwards);
    digitalWrite(PWM_LEFT, model.leftActive);
    
    digitalWrite(IN_RIGHT, model.isForwards);
    digitalWrite(PWM_RIGHT, model.rightActive);

    digitalWrite(LED_BUILTIN_1, model.leftActive);
    digitalWrite(LED_BUILTIN_2, model.rightActive);
}

void loop()
{
    if (nrf24.available())
    {
        updateModelInputs();
        move();
    }

    delay(50);
}
