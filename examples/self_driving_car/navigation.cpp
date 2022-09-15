#include "navigation.h"
#include "rudiron/tasks_timer.h"

void setup_navigation()
{
    setup_rangefinder();
    setup_driver();
}

/// @brief Определение свободного от препятствий направления движения
/// @return left или right, если свободно, backwards, если нет
DirectionsEnum findFreeDirection()
{
    DirectionsEnum direction;
    bool randomSide = rand() % 2;
    direction = randomSide ? DirectionsEnum::left : DirectionsEnum::right;

    scan_range(direction);
    if (isObstacle(direction))
    {
        direction = opposite_direction(direction);
        scan_range(direction);

        if (isObstacle(direction))
        {
            return DirectionsEnum::backwards;
        }
        else
        {
            return direction;
        }
    }
    else
    {
        return direction;
    }
}

/// @brief Перемещение
/// @param direction направление
/// @param timeout максимальное время
/// @param scanObstacle проверка на препятствие
void move(DirectionsEnum direction, int timeout, bool scanObstacle, DriverSpeedEnum speed)
{
    auto time_end = millis() + timeout;
    drive_towards(direction, speed);

    while (millis() < time_end)
    {
        if (scanObstacle)
        {
            scan_range(DirectionsEnum::straight);

            if (isObstacle(DirectionsEnum::straight))
            {
                break;
            }
        }
    }

    drive_stop();
}

void loop_navigation()
{
    scan_range(DirectionsEnum::straight);
    if (!isObstacle(DirectionsEnum::straight))
    {
        move(DirectionsEnum::straight, METER_MS / (int)DriverSpeedEnum::first, true, DriverSpeedEnum::first);
    }

    DirectionsEnum freeDirection = findFreeDirection();
    if (freeDirection != DirectionsEnum::backwards)
    {
        move(freeDirection, METER_MS / 5 / (int)DriverSpeedEnum::second, false, DriverSpeedEnum::second);
    }
    else
    {
        while (freeDirection == DirectionsEnum::backwards)
        {
            move(DirectionsEnum::backwards, METER_MS / (int)DriverSpeedEnum::first, false, DriverSpeedEnum::first);
            freeDirection = findFreeDirection();
        }
    }
}
