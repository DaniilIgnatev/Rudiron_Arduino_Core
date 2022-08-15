#include "tank_receiver.h"

#ifndef TRANSMITTER

#define leftEngineF 12
#define leftEngineB 13
#define rightEngineF 14
#define rightEngineB 15

using namespace Rudiron;

void configEngines()
{
    pinMode(leftEngineF, OUTPUT);
    pinMode(leftEngineB, OUTPUT);
    pinMode(rightEngineF, OUTPUT);
    pinMode(rightEngineB, OUTPUT);
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

bool isForwards = false;

void turn(bool left, bool active)
{
    digitalWrite(leftEngineB, !(isForwards == left));
    digitalWrite(rightEngineB, (isForwards == left));

    digitalWrite(leftEngineF, (isForwards == left));
    digitalWrite(rightEngineF, !(isForwards == left));

    digitalWrite(left ? LED_BUILTIN_1 : LED_BUILTIN_2, active);
}

void tank_loop()
{
    if (nrf24.available())
    {
        isForwards = nrf24.read();
        bool leftActive = nrf24.read();
        bool rightActive = nrf24.read();

        turn(true, leftActive);
        turn(false, rightActive);
    }

    delay(50);
}

#endif
