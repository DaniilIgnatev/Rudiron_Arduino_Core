#include "tank_receiver.h"

#ifndef TRANSMITTER

#define leftEngineF 12
#define leftEngineB 13
#define rightEngineF 14
#define rightEngineB 15

bool isForwards = false;

void configEngines(){
    pinMode(leftEngineF, OUTPUT);
    pinMode(leftEngineB, OUTPUT);
    pinMode(rightEngineF, OUTPUT);
    pinMode(rightEngineB, OUTPUT);
}

void move(){
    digitalWrite(leftEngineB, !isForwards);
    digitalWrite(rightEngineB, !isForwards);

    digitalWrite(leftEngineF, isForwards);
    digitalWrite(rightEngineF, isForwards);
}

void turn(bool left){
    digitalWrite(leftEngineB, !(isForwards == left));
    digitalWrite(rightEngineB, (isForwards == left));

    digitalWrite(leftEngineF, (isForwards == left));
    digitalWrite(rightEngineF, !(isForwards == left));
}

void tank_setup(){
    if (!nrf24.begin(true)){
        Serial.println("Nrf24 Error!");
        return;
    }

    configEngines();
}

void tank_loop(){
    if (nrf24.available()){
        isForwards = nrf24.read();
        bool leftActive = nrf24.read();
        bool rightActive = nrf24.read();

        if (leftActive && rightActive){
            move();
        }
        else{
            if (leftActive){
                turn(true);
            }
            else{
                turn(false);
            }
        }
    }

    if (Serial.available()){
        String data = Serial.readString();
        Serial.print(data);
    }

    delay(50);
}

#endif
