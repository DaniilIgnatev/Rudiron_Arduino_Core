#include "tank_receiver.hpp"

#ifndef TRANSMITTER

void tank_setup(){
    if (!nrf24.begin(true)){
        Serial.println("Nrf24 Error!");
    }
}

void tank_loop(){
    if (nrf24.available()){
        uint8_t data = nrf24.read();
        Serial.println(data);
    }
}

#endif
