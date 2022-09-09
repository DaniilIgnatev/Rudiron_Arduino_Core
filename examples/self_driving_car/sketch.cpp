#include "navigation.h"

#define TESTS

#ifdef TESTS
#include "tests.h"
#endif

void setup()
{
    int seed = analogRead(0);
    randomSeed(seed);
    setup_navigation();

#ifdef TESTS
    run_tests();
#endif
}

void loop()
{
    loop_navigation();
}
