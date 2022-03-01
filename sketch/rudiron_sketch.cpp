#include "Arduino.h"


#include "rudiron/uart.h"
#include "rudiron/can.h"
#include "rudiron/gpio.h"

using namespace Rudiron;


void setup(){
    GPIO::configPinOutput(Rudiron::PORT_PIN_A2);
    GPIO::configPinOutput(Rudiron::PORT_PIN_A4);
}


void loop(){
    GPIO::writePin(Rudiron::PORT_PIN_A2, true);
    GPIO::writePin(Rudiron::PORT_PIN_A4, true);
    delay(1000);
    GPIO::writePin(Rudiron::PORT_PIN_A2, false);
    GPIO::writePin(Rudiron::PORT_PIN_A4, false);
    delay(1000);
}
