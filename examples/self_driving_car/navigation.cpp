#include "navigation.h"
#include "rudiron/tasks_timer.h"

void setup_navigation()
{
    setup_rangefinder();
    setup_driver();
}

/// @brief Определение свободного от препятствий направления движения
/// @return left или right, если свободно, backwards, если нет
DirectionsEnum findFreeDirection(){
    DirectionsEnum direction = (DirectionsEnum)(rand() % 2);

    scan_range(direction);
    if (isObstacle(direction)){
        direction = opposite_direction(direction);

        if (isObstacle(direction)){
            return DirectionsEnum::backwards;
        }
        else{
            return direction;
        }
    }
    else{
        return direction;
    }
}

/// @brief Перемещение
/// @param direction направление
/// @param timeout максимальное время
/// @param scanObstacle проверка на препятствие
void move(DirectionsEnum direction, int timeout, bool scanObstacle){
    auto time_end = millis() + timeout;
    drive_towards(direction, DriverSpeedEnum::third);

    while (millis() < time_end)
    {
        if (scanObstacle){
            scan_range(DirectionsEnum::straight);
            if (isObstacle(DirectionsEnum::straight)){
                break;
            }
        }
    }
    
    drive_stop();
}

void loop_navigation()
{
    scan_range(DirectionsEnum::straight);
    if (!isObstacle(DirectionsEnum::straight)){
        move(DirectionsEnum::straight, METER_MS, true);
    }

    DirectionsEnum freeDirection = findFreeDirection();
    while (freeDirection == DirectionsEnum::backwards)
    {
        move(DirectionsEnum::backwards, METER_MS, false);
        freeDirection = findFreeDirection();
    }
}