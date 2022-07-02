#include "tank_receiver.hpp"

#ifndef TRANSMITTER

void tank_setup(){
    // nRF24::begin(true, false);
}

void tank_loop(){
    // if (nRF24::available()){
    //     nRF24::read();
    //     if (nRF24::rx_changed()){
    //         for (int i = 0; i < nRF24::payload_length;i++){
    //             Serial.print(nRF24::rx_data[i]);
    //             Serial.print(";");
    //         }
    //         Serial.println();
    //     }
    // }
}

#endif
