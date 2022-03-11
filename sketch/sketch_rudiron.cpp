#include "Arduino.h"
#include "rudiron/gpio.h"


void setup(){
    GPIO::configPinOutput(LED_BUILTIN_1);
    GPIO::configPinOutput(LED_BUILTIN_2);

    GPIO::configPinInputPullDown(BUTTON_BUILTIN_1);
    GPIO::configPinInputPullDown(BUTTON_BUILTIN_2);
    GPIO::configPinInputPullDown(BUTTON_BUILTIN_3);
}


void loop(){
    int b = 0;

    if (GPIO::readButton_1()){
        b = 1;
    }
    if (GPIO::readButton_2()){
        b = 2;
    }
    if (GPIO::readButton_3()){
        b = 3;
    }

    GPIO::writeLED_1(true);
    GPIO::writeLED_2(true);
    CLK::delay_millis(1000);
    GPIO::writeLED_1(false);
    GPIO::writeLED_2(false);
    CLK::delay_millis(1000);
}
