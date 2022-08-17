#include "sketch.h"

bool callback1(void*){
    digitalWrite(LED_BUILTIN_1, !digitalRead(LED_BUILTIN_1));
    return true;
}

bool callback2(void*){
    digitalWrite(LED_BUILTIN_2, !digitalRead(LED_BUILTIN_2));
    return true;
}

void setup()
{
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED_BUILTIN_1, true);
        digitalWrite(LED_BUILTIN_2, true);
        delay(100);
        digitalWrite(LED_BUILTIN_1, false);
        digitalWrite(LED_BUILTIN_2, false);
        delay(100);
    }

    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");

    tasksTimer.start_every_millis(1000, callback1);
    tasksTimer.start_every_millis(2000, callback2);

}


bool pressed1 = false;

bool pressed2 = false;

bool pressed3 = false;

void loop()
{
    pressed1 = digitalRead(BUTTON_BUILTIN_1);
    pressed2 = digitalRead(BUTTON_BUILTIN_2);
    pressed3 = digitalRead(BUTTON_BUILTIN_3);

}
