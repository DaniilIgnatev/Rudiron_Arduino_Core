#include "tests.h"

void test_driver(){
    drive_towards(DirectionsEnum::left);
    delay(200);
    drive_towards(DirectionsEnum::right);
    delay(200);
    drive_towards(DirectionsEnum::straight);
    delay(200);
    drive_towards(DirectionsEnum::backwards);
    delay(200);
    driver_stop();
}

void test_rangefinder(){
    scan_range(DirectionsEnum::left);
    scan_range(DirectionsEnum::right);
    scan_range(DirectionsEnum::straight);
}

void run_tests(){
    test_driver();
    test_rangefinder();
}