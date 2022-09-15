#include "Arduino.h"
#include "Servo.h"

#define SERVO_PIN 8

Servo servo;

void setup(){
    servo.attach(SERVO_PIN, 470, 2500);
}

void loop(){
    servo.write(0);
    delay(2000);
    servo.write(90);
    delay(2000);
    servo.write(180);
    delay(2000);
}
