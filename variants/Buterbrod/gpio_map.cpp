//
// Created by daniil on 23.02.2022.
//

#include "gpio_map.h"
#include "rudiron/gpio.h"
#include "pins_arduino.h"

using namespace Rudiron;

const PortPinName Rudiron::pinMap[NUM_DIGITAL_PINS]{
        PORT_PIN_F3,//0
        PORT_PIN_F2,//1
        PORT_PIN_F1,//2
        PORT_PIN_F0,//3
        PORT_PIN_A1,//4
        PORT_PIN_A2,//5
        PORT_PIN_A3,//6
        PORT_PIN_A4,//7
        PORT_PIN_A5,//8
        PORT_PIN_B8,//9
        PORT_PIN_B7,//10
        PORT_PIN_B6,//11
        PORT_PIN_B5,//12
        PORT_PIN_B4,//13
        PORT_PIN_B3,//14
        PORT_PIN_B0,//15
        PORT_PIN_C0,//16
        PORT_PIN_C1,//17
        PORT_PIN_C2,//18
        PORT_PIN_D6,//19
        PORT_PIN_D5,//20
        PORT_PIN_D3,//21
        PORT_PIN_D2,//22
        PORT_PIN_D4,//23
        PORT_PIN_D7,//24
        PORT_PIN_E0,//25
        PORT_PIN_E1,//26
        PORT_PIN_E2,//27
        PORT_PIN_E3,//28
        PORT_PIN_B10,//29
        PORT_PIN_B9,//30
        PORT_PIN_E6,//31
        PORT_PIN_E7,//32
        PORT_PIN_A0,//33
        PORT_PIN_D0,//34
        PORT_PIN_D1,//35
};


const PortPinName Rudiron::pinADCMap[NUM_ANALOG_INPUTS]{
        PORT_PIN_D6,//A0
        PORT_PIN_D5,//A1
        PORT_PIN_D3,//A2
        PORT_PIN_D2,//A3
        PORT_PIN_D4,//A4
        PORT_PIN_D7,//A5
};