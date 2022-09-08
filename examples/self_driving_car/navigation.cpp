#include "navigation.h"

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

void loop_navigation()
{
    scan_range(DirectionsEnum::straight);
    bool obstacle = isObstacle(DirectionsEnum::straight);

    if (isObstacle){
        
    }
    else{
        drive(DirectionsEnum::straight);
    }
}