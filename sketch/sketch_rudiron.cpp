#include "Arduino.h"
#include "rudiron/gpio.h"
#include "nrf24l01/nrf24.h"
#include "rudiron/timer.h"



bool pressed1 = false;

bool pressed2 = false;

bool pressed3 = false;

uint8_t data[32] = {0};


void reverse()
{
    data[0] = !data[0];
}


void turnLeft(bool stop)
{
    digitalWrite(LED_BUILTIN_1, stop);
    data[1] = stop;
}


void turnRight(bool stop)
{
    digitalWrite(LED_BUILTIN_2, stop);
    data[2] = stop;
}


void setup()
{
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    nRF24::begin(false, false);

    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    for (int i = 0; i < 3; i++){
        digitalWrite(LED_BUILTIN_1, true);
        digitalWrite(LED_BUILTIN_2, true);
        delay(100);
        digitalWrite(LED_BUILTIN_1, false);
        digitalWrite(LED_BUILTIN_2, false);
        delay(100);
    }


    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");

    Timer::getTimer1()->start();
    Timer::getTimer1()->PWM_setup(500);
    Timer::getTimer1()->PWM_start(PORT_PIN_A1, 50);
}


void loop()
{
    turnLeft(digitalRead(BUTTON_BUILTIN_1));

    if (!pressed2 && digitalRead(BUTTON_BUILTIN_2))
    {
        pressed2 = true;

        reverse();
    }
    else
    {
        pressed2 = false;
    }

    turnRight(digitalRead(BUTTON_BUILTIN_3));
    
    nRF24::write(data);

    if (Serial.available()){
        String s = Serial.readString();
        Serial.println(s);
    }

    delay(5);
}
