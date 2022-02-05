#include "gpiomap.hpp"



#if defined RUDIRON_BUTERBROD_rev5
const PortPinName GPIOMap::pinMap[pinMapLength] = {
    PIN_F3,//0
    PIN_F2,//1
    PIN_F1,//2
    PIN_F0,//3
    PIN_A1,//4
    PIN_A2,//5
    PIN_A3,//6
    PIN_A4,//7
    PIN_A5,//8
    PIN_B8,//9
    PIN_B7,//10
    PIN_B6,//11
    PIN_B5,//12
    PIN_B4,//13
    PIN_B3,//14
    PIN_B0,//15
    PIN_C0,//16
    PIN_C1,//17
    PIN_C2,//18
    PIN_D6,//19
    PIN_D5,//20
    PIN_D3,//21
    PIN_D2,//22
    PIN_D4,//23
    PIN_D7,//24
    PIN_E0,//25
    PIN_E1,//26
    PIN_E2,//27
    PIN_E3,//28
    PIN_B10,//29
    PIN_B9,//30
    PIN_E6,//31
    PIN_E7,//32
    PIN_A0,//33
    PIN_D0,//34
    PIN_D1,//35
};
#endif


uint8_t GPIOMap::getPinNumber(PortPinName pinName)
{
    for (uint8_t i = 0; i < pinMapLength; i++){
        if (pinName == pinMap[i]){
            return i;
        }
    }

    //ОШИБКА!!!
    return 0;
}


PortPinName GPIOMap::getPinName(uint8_t pinNumber)
{
    if (pinNumber >= pinMapLength){
        return PIN_NONE;
    }

    return pinMap[pinNumber];
}


#if defined RUDIRON_BUTERBROD_rev5
const PortPinName GPIOMap::pinADCMap[pinADCMapLength] = {
    PIN_D6,//A0
    PIN_D5,//A1
    PIN_D3,//A2
    PIN_D2,//A3
    PIN_D4,//A4
    PIN_D7,//A5
};
#endif


PortPinName GPIOMap::getADCPinName(uint8_t pinNumber)
{
    if (pinNumber >= pinADCMapLength){
        return PIN_NONE;
    }

    return pinADCMap[pinNumber];
}


