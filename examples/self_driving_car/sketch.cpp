#include "navigation.h"

void setup()
{
    int seed = analogRead(0);
    randomSeed(seed);
    setup_navigation();
}

void loop()
{
    loop_navigation();
}
