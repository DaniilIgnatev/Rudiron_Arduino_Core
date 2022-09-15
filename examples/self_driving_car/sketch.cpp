#include "navigation.h"

// #define TESTS

#ifdef TESTS
#include "tests.h"
#endif

bool enabled = false;
bool button2 = false;

void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);

    int seed = analogRead(0);
    randomSeed(seed);
    setup_navigation();

#ifdef TESTS
    run_tests();
#endif
}

void loop()
{
    bool newButton2 = digitalRead(BUTTON_BUILTIN_2);
    if (newButton2 && !button2)
    {
        enabled = !enabled;
    }
    button2 = newButton2;

#ifndef TESTS
    if (enabled)
    {
        loop_navigation();
    }
#endif
}
