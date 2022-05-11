#include "Arduino.h"
#include "rudiron/gpio.h"
#include "nrf24l01/nrf24.h"
#include "rudiron/timer.h"

#include "rudiron/can.h"


// #define NRF_TRANSMITTER
// #define NRF_RECEIVER

#if defined(NRF_RECEIVER) || defined(NRF_TRANSMITTER)
uint8_t nrf_buffer[32] = {0};
#endif


#ifdef NRF_TRANSMITTER

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

#elif defined(NRF_RECEIVER)

void reverse()
{
    
}

void turnLeft()
{
    digitalWrite(LED_BUILTIN_1, nrf_buffer[1]);
}

void turnRight()
{
    digitalWrite(LED_BUILTIN_2, nrf_buffer[2]);
}
#endif


void setup()
{
    tone(8, 500);

    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

#ifdef NRF_TRANSMITTER
    nRF24::begin(false, false);
#elif defined(NRF_RECEIVER)
     nRF24::begin(true, false);
#endif

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
    CAN::getCAN1()->setActiveID(2);

    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");
}


bool pressed1 = false;

bool pressed2 = false;

bool pressed3 = false;

void loop()
{
    String can_rx_buffer;

#ifdef NRF_TRANSMITTER
    turnLeft(digitalRead(BUTTON_BUILTIN_1));
#endif

    if (digitalRead(BUTTON_BUILTIN_1))
    {
        if (!pressed1){
            pressed1 = true;

            CAN::getCAN1()->setActiveID(1);
            CAN::getCAN1()->println("Привет!");
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

#if defined(NRF_RECEIVER) || defined(NRF_TRANSMITTER)
            reverse();
#endif
            can_rx_buffer = CAN::getCAN1()->readString();
            Serial.print(can_rx_buffer);
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
            CAN::getCAN1()->println("1РУДИРОН Бутерброд!2РУДИРОН Бутерброд!3РУДИРОН Бутерброд!4РУДИРОН Бутерброд!5РУДИРОН Бутерброд!6РУДИРОН Бутерброд!7РУДИРОН Бутерброд!8РУДИРОН Бутерброд!");
        }
    }
    else
    {
        pressed3 = false;
    }

#ifdef NRF_TRANSMITTER
    turnRight(digitalRead(BUTTON_BUILTIN_3));
    nRF24::write(nrf_buffer);
#elif defined(NRF_RECEIVER)
    nRF24::read(nrf_buffer);
#endif
    

    if (Serial.available()){
        String s = Serial.readString();
        Serial.println(s);
    }

    delay(100);
}
