#include "tank_receiver.h"

#ifndef TRANSMITTER

void tank_setup(){
    if (!nrf24.begin(true)){
        Serial.println("Nrf24 Error!");
    }
}

void tank_loop(){
    if (nrf24.available()){
        String data = nrf24.readString();
        Serial.print(data);
    }
}

#endif
