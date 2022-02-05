/*
This file is part of DIBotQBS.

DIBotQBS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DIBotQBS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DIBotQBS.  If not, see <https://www.gnu.org/licenses/>.
*/



#ifndef GPIOMAP_H
#define GPIOMAP_H


#include "gpio.h"


///Описывает связь функций ввода/вывода и пинов
class GPIOMap
{
public:

#if defined RUDIRON_BUTERBROD_rev5
   static const uint8_t pinMapLength = 36;
   static const uint8_t pinADCMapLength = 6;
#endif

private:

    static const PortPinName pinMap[pinMapLength];


    static const PortPinName pinADCMap[pinADCMapLength];

public:

    static uint8_t getPinNumber(PortPinName pinName);


    static PortPinName getPinName(uint8_t pinNumber);


    static PortPinName getADCPinName(uint8_t pinNumber);

};

#endif // GPIOMAP_H
