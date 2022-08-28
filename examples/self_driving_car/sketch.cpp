#include "sketch.h"

void setup()
{
    setup_movement();
    setup_distance();
}

int angle_to_delayMS(int angle)
{
    return 1000;
}

int turn(DirectionsEnum direction, int angle = 90)
{
    updateMovement(direction);
    int delayTimeMS = angle_to_delayMS(angle);
    delay(delayTimeMS);
}

int distance_to_delayMS(int sm){
    return 1000;
}

void move(DirectionsEnum direction, int distance){

}

void loop()
{
    scan_distance(DirectionsEnum::straight);

    if (isObstacle(DirectionsEnum::straight))
    {
        DirectionsEnum direction = (bool)random(0, 1) ? DirectionsEnum::right : DirectionsEnum::left;

        scan_distance(direction);
        if (isObstacle(direction))
        {
            scan_distance(direction == DirectionsEnum::left ? DirectionsEnum::right : DirectionsEnum::left);
        }
        else
        {

            return;
        }

        bool obstacle = true;
        while (isObstacle)
        {
            scan_distance(direction);
            turn(direction);

            scan_distance(DirectionsEnum::straight);
            obstacle = isObstacle(DirectionsEnum::straight);
        }
    }
    else
    {
    }
}
