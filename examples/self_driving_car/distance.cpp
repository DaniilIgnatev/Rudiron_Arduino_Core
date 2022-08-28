#include "distance.h"
#include "Servo.h"

struct DistanceModel
{
    float direction_distances[3] = {0};

    inline bool isObstacle(DirectionsEnum direction_index)
    {
        return direction_distances[(int)direction_index] <= 30 && direction_distances[(int)direction_index] > 0;
    }

    void setDistance(DirectionsEnum direction, int distance)
    {
        direction_distances[(int)direction] = distance;
    }
};

DistanceModel navigation_model;

bool isObstacle(DirectionsEnum direction)
{
    return navigation_model.isObstacle(direction);
}

#define HCSR04_TRIG_PIN 9
#define HCSR04_ECHO_PIN 10

void setup_sensor()
{
    pinMode(HCSR04_TRIG_PIN, OUTPUT);
    pinMode(HCSR04_ECHO_PIN, INPUT);
}

#define SERVO_PIN 9

Servo servo;

void setup_servo()
{
    servo.attach(SERVO_PIN);
}

void setup_distance()
{
    setup_sensor();
    setup_servo();
}

void turnHead(DirectionsEnum direction)
{
    int angle = (int)direction * 45;
    servo.write(angle);
}

void updateDistance(DirectionsEnum direction)
{
    digitalWrite(HCSR04_TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(HCSR04_TRIG_PIN, HIGH);

    //Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
    delayMicroseconds(10);
    digitalWrite(HCSR04_TRIG_PIN, LOW);

    //Время задержки акустического сигнала на эхолокаторе.
    int duration = pulseIn(HCSR04_ECHO_PIN, HIGH);

    //Расстояние в сантиметрах
    float distance = (duration / 2) / 29.1;
    navigation_model.setDistance(direction, distance);
}

void scan_distance(DirectionsEnum direction)
{
    turnHead(direction);
    updateDistance(direction);
}