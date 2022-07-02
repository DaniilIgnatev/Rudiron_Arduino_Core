#include "tank_receiver.hpp"

#ifndef TRANSMITTER

void tank_setup(){
    if (!nrf24.begin(true)){
        Serial.println("Nrf24 Error!");
    }
}

void tank_loop(){
    if (nrf24.available()){
        char data = nrf24.read();
        Serial.print(data);
    }
}

#endif
