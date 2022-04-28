#include "Arduino.h"
#include "rudiron/gpio.h"
#include "nrf24l01/nrf24.h"
#include "rudiron/timer.h"

#include "rudiron/can.h"


uint8_t nrf_buffer[32] = {0};


void reverse()
{
    nrf_buffer[0] = !nrf_buffer[0];
}


void turnLeft(bool stop)
{
    digitalWrite(LED_BUILTIN_1, stop);
    nrf_buffer[1] = stop;
}


void turnRight(bool stop)
{
    digitalWrite(LED_BUILTIN_2, stop);
    nrf_buffer[2] = stop;
}


void setup()
{
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    // nRF24::begin(false, false);

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

    CAN::getCAN1()->begin();

    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");

    CAN::getCAN1()->write(5);
}


int can_rx_buffer = -1;

bool pressed1 = false;

bool pressed2 = false;

bool pressed3 = false;

void loop()
{
    turnLeft(digitalRead(BUTTON_BUILTIN_1));

    if (digitalRead(BUTTON_BUILTIN_1))
    {
        if (!pressed1){
            pressed1 = true;

            CAN::getCAN1()->setActiveID(1);
            CAN::getCAN1()->write(10);
        }
    }
    else
    {
        pressed1 = false;
    }

    if (digitalRead(BUTTON_BUILTIN_2))
    {
        if (!pressed2){
            pressed2 = true;

            reverse();
            can_rx_buffer = CAN::getCAN1()->read();
        }
    }
    else
    {
        pressed2 = false;
    }

    if (digitalRead(BUTTON_BUILTIN_3))
    {
        if (!pressed3){
            pressed3 = true;

            CAN::getCAN1()->setActiveID(2);
            CAN::getCAN1()->write(20);
        }
    }
    else
    {
        pressed3 = false;
    }

    turnRight(digitalRead(BUTTON_BUILTIN_3));
    
    // nRF24::write(nrf_buffer);

    if (Serial.available()){
        String s = Serial.readString();
        Serial.println(s);
    }

    delay(100);
}
