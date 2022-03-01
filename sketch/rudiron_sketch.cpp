#include "Arduino.h"
#include "rudiron/gpio.h"


void setup(){
    GPIO::configPinOutput(LED_BUILTIN_1);
    
}


void loop(){
    GPIO::writeLED_1(true);
    CLK::delay_millis(1000);
    GPIO::writeLED_1(false);
    CLK::delay_millis(1000);
}
