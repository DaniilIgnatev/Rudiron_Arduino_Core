#include "rangefinder.h"
#include "Servo.h"

struct RangefinderModel
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

RangefinderModel rangefinder_model;

#define OBSTACLE_TRUE_PIN 4
#define OBSTACLE_FALSE_PIN 6

void indicateObstacle(bool obstacle)
{
    digitalWrite(OBSTACLE_TRUE_PIN, obstacle);
    digitalWrite(OBSTACLE_FALSE_PIN, !obstacle);
}

void setup_indicator()
{
    pinMode(OBSTACLE_TRUE_PIN, OUTPUT);
    pinMode(OBSTACLE_FALSE_PIN, OUTPUT);
}

bool isObstacle(DirectionsEnum direction)
{
    bool obstacle = rangefinder_model.isObstacle(direction);
    indicateObstacle(obstacle);
    return obstacle;
}

#define HCSR04_TRIG_PIN 14
#define HCSR04_ECHO_PIN 15

void setup_sensor()
{
    pinMode(HCSR04_TRIG_PIN, OUTPUT);
    pinMode(HCSR04_ECHO_PIN, INPUT);
}

#define SERVO_PIN 5
//Серво двигатель установлен в обратную сторону
#define SERVO_INVERSED true
//Угол поворота по сторонам
#define SERVO_ROTATION 70

Servo servo;

void setup_servo()
{
    servo.attach(SERVO_PIN, 470, 2500);
}

void setup_rangefinder()
{
    setup_indicator();
    setup_sensor();
    setup_servo();
}

void turnHead(DirectionsEnum direction, bool wait)
{
    int angle;
    int currentAngle = servo.read();

    switch (direction)
    {
    case DirectionsEnum::left:
        angle = SERVO_INVERSED ? 90 + SERVO_ROTATION : 90 - SERVO_ROTATION;
        break;
    case DirectionsEnum::right:
        angle = SERVO_INVERSED ? 90 - SERVO_ROTATION : 90 + SERVO_ROTATION;
        break;
    default:
        angle = 90;
        break;
    }

    servo.write(angle);

    if (wait)
    {
        //Дать время сервоприводу повернуться
        int angleRange = angle - currentAngle;
        if (angleRange != 0)
        {
            if (angleRange < 0)
            {
                angleRange = -angleRange;
            }

            int delay_ms = angleRange << 2;
            delay(delay_ms);
        }
    }
}

float measureDistance()
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
    return distance;
}

void scan_range(DirectionsEnum direction)
{
    turnHead(direction, true);
    float distance = measureDistance();
    rangefinder_model.setDistance(direction, distance);
    turnHead(DirectionsEnum::straight, false);
}
