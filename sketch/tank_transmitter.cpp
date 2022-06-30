#include "tank_transmitter.hpp"

#ifdef TRANSMITTER

void tank_setup(){
    nRF24::begin(false, false);
}

void tank_loop(){
    if (pressed1){
        nRF24::tx_data[0] = nRF24::tx_data[0] - 1;
    }

    if (pressed3){
        nRF24::tx_data[0] = nRF24::tx_data[0] + 1;
    }

    if (pressed2){
        nRF24::write();
    }
}

#endif