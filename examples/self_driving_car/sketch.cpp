#include "navigation.h"

#define TESTS

#ifdef TESTS
#include "tests.h"
#endif

void setup()
{
    Serial.begin(115200);
    
    int seed = analogRead(0);
    randomSeed(seed);
    setup_navigation();

#ifdef TESTS
    run_tests();
#endif
}

void loop()
{
    #ifndef TESTS
    loop_navigation();
    #endif
}
