#include "tank_transmitter.h"

#ifdef TRANSMITTER

void tank_setup(){
    if (!nrf24.begin(false)){
        Serial.println("Nrf24 Error!");
    }
}

uint8_t c = 0;

void tank_loop(){
    while (Serial.available()){
        String data = Serial.readString();
        nrf24.print(data);
    }

    if (pressed1){
        c--;
    }

    if (pressed3){
        c++;
    }

    if (pressed2){
        if (nrf24.write(c)){
            Serial.println(c);
        }
    }
}

#endif