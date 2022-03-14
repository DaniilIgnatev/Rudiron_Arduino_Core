#include "Arduino.h"
#include "rudiron/gpio.h"
#include "nrf24l01/nrf24.h"


bool pressed1 = false;

bool pressed2 = false;

bool pressed3 = false;


uint8_t data[32] = {0};


void reverse()
{
    data[0] = !data[0];
}


void turnLeft(bool stop)
{
    GPIO::writeLED_1(stop);
    data[1] = stop;
    if (stop){
        int a = 5;
    }
}


void turnRight(bool stop)
{
    GPIO::writeLED_2(stop);
    data[2] = stop;
}


void setup()
{
    GPIO::configPinOutput(LED_BUILTIN_1);
    GPIO::configPinOutput(LED_BUILTIN_2);

    nRF24::begin(false, false);

    GPIO::configPinInputPullDown(BUTTON_BUILTIN_1);
    GPIO::configPinInputPullDown(BUTTON_BUILTIN_2);
    GPIO::configPinInputPullDown(BUTTON_BUILTIN_3);

    for (int i = 0; i < 3; i++){
        GPIO::writeLED_1(true);
        GPIO::writeLED_2(true);
        CLK::delay_millis(1000);
        GPIO::writeLED_1(false);
        GPIO::writeLED_2(false);
        CLK::delay_millis(1000);
    }
}


void loop()
{
    turnLeft(GPIO::readButton_1());

    if (!pressed2 && GPIO::readButton_2())
    {
        pressed2 = true;

        reverse();
    }
    else
    {
        pressed2 = false;
    }

    turnRight(GPIO::readButton_3());

    nRF24::write(data);
    CLK::delay_millis(100);
}
