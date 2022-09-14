#include "tests.h"

void test_driver()
{
    drive_towards(DirectionsEnum::left, DriverSpeedEnum::first);
    delay(500);
    drive_towards(DirectionsEnum::right, DriverSpeedEnum::first);
    delay(500);
    drive_towards(DirectionsEnum::backwards, DriverSpeedEnum::second);
    delay(500);
    drive_towards(DirectionsEnum::straight, DriverSpeedEnum::third);
    delay(500);

    drive_stop();
}

void test_rangefinder()
{
    scan_range(DirectionsEnum::left);
    scan_range(DirectionsEnum::right);
    scan_range(DirectionsEnum::straight);
}

void run_tests()
{
    // test_driver();
    // test_rangefinder();
}