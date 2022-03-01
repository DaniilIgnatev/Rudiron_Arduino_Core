#include "Arduino.h"
#include "rudiron/gpio.h"


void setup(){
    GPIO::configPinOutput(LED_BUILTIN_1);
    GPIO::configPinOutput(LED_BUILTIN_2);
}


void loop(){
    GPIO::writeLED_1(true);
    GPIO::writeLED_2(true);
    CLK::delay_millis(1000);
    GPIO::writeLED_1(false);
    GPIO::writeLED_2(false);
    CLK::delay_millis(1000);
}
