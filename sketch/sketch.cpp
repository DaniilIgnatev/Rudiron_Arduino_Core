#include "sketch.hpp"
#include "tank_receiver.hpp"
#include "tank_transmitter.hpp"


void setup()
{
    CLK::setCPUSpeed(CLK_Speed::low);
    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

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

    tank_setup();
}

bool pressed1 = false;

bool pressed2 = false;

bool pressed3 = false;

void loop()
{
    if (digitalRead(BUTTON_BUILTIN_1))
    {
        if (!pressed1){
            pressed1 = true;
            bool b = digitalRead(LED_BUILTIN_1);
            digitalWrite(LED_BUILTIN_1, !b);
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
            bool b = digitalRead(LED_BUILTIN_1);
            digitalWrite(LED_BUILTIN_1, !b);

            b = digitalRead(LED_BUILTIN_2);
            digitalWrite(LED_BUILTIN_2, !b);
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
            bool b = digitalRead(LED_BUILTIN_2);
            digitalWrite(LED_BUILTIN_2, !b);
        }
    }
    else
    {
        pressed3 = false;
    }

    tank_loop();

    while (Serial.available()){
        Serial.print((char)Serial.read());
    }

    delay(100);
}
