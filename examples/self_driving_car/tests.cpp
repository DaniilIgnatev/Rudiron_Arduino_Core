#include "tests.h"

void test_driver()
{
    Serial.println("Start test_driver");

    Serial.println("drive_towards(DirectionsEnum::left, DriverSpeedEnum::first);");
    drive_towards(DirectionsEnum::left, DriverSpeedEnum::first);
    delay(500);

    Serial.println("drive_towards(DirectionsEnum::right, DriverSpeedEnum::first);");
    drive_towards(DirectionsEnum::right, DriverSpeedEnum::first);
    delay(500);

    Serial.println("drive_towards(DirectionsEnum::backwards, DriverSpeedEnum::second);");
    drive_towards(DirectionsEnum::backwards, DriverSpeedEnum::second);
    delay(500);

    Serial.println("drive_towards(DirectionsEnum::straight, DriverSpeedEnum::third);");
    drive_towards(DirectionsEnum::straight, DriverSpeedEnum::third);
    delay(500);

    Serial.println("drive_stop();");
    drive_stop();
    Serial.println("End test_driver");
}

void test_MeasureDistance(){
    Serial.println("Start test_MeasureDistance");
    for (int i = 0; i < 10; i++)
    {
        float distance = measureDistance();
        Serial.print("Distance: ");
        Serial.println(distance);

        delay(500);
    }
    Serial.println("End test_MeasureDistance");
}

void test_rangefinder()
{
    Serial.println("Start test_rangefinder");

    Serial.println("scan_range(DirectionsEnum::left);");
    scan_range(DirectionsEnum::left);

    Serial.println("scan_range(DirectionsEnum::right);");
    scan_range(DirectionsEnum::right);

    Serial.println("scan_range(DirectionsEnum::straight);");
    scan_range(DirectionsEnum::straight);

    Serial.println("End test_rangefinder");
}

void run_tests()
{
    Serial.println("Start run_tests");

    // test_driver();
    // test_MeasureDistance();
    test_rangefinder();

    Serial.println("End run_tests");
}