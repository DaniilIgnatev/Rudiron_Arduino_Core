#include "sketch.h"
#include "tank_receiver.h"
#include "tank_transmitter.h"


void setup()
{
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
    pressed1 = digitalRead(BUTTON_BUILTIN_1);
    pressed2 = digitalRead(BUTTON_BUILTIN_2);
    pressed3 = digitalRead(BUTTON_BUILTIN_3);
    
    tank_loop();
}
