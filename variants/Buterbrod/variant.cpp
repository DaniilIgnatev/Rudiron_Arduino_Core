//
// Created by daniil on 23.02.2022.
//

#include "variant.h"

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }


void initVariant() {


}

void setupUSB() {


}

//добавить обработку прерываний rx
//Serial1._rx_complete_irq();

//добавить обработку прерываний tx
//Serial1._tx_udr_empty_irq();

//HardwareSerial Serial1;

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
//bool Serial1_available() {
//    return Serial1.available();
//}