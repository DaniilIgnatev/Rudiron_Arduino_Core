#include "tank_transmitter.h"

#ifdef TRANSMITTER

uint8_t package[3];

void tank_setup(){
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    if (!nrf24.begin(false)){
        Serial.println("Nrf24 Error!");
        return;
    }
}

bool pressed2_last = false;
bool isForwards = false;

void tank_loop(){
    while (Serial.available()){
        String data = Serial.readString();
        nrf24.print(data);
    }

    if (pressed1){
        package[1] = true;
    }

    if (pressed3){
        package[2] = true;
    }

    if (!pressed2_last && pressed2){
        isForwards = !isForwards;
        digitalWrite(LED_BUILTIN_1, !isForwards);
        digitalWrite(LED_BUILTIN_2, isForwards);
    }
    pressed2_last = pressed2;
    package[0] = isForwards;
    
    if (nrf24.availableForWrite()){
        nrf24.write(package, 3);
    }

    delay(100);
}

#endif