#include "movement.h"

#define leftEngineFrontPin 12
#define leftEngineBackPin 13
#define rightEngineFrontPin 14
#define rightEngineBackPin 15

void setup_movement()
{
    pinMode(leftEngineFrontPin, OUTPUT);
    pinMode(leftEngineBackPin, OUTPUT);
    pinMode(rightEngineFrontPin, OUTPUT);
    pinMode(rightEngineBackPin, OUTPUT);
}

struct MovementModel
{
public:
    bool isForwards = false;
    bool leftActive = false;
    bool rightActive = false;

    bool leftEngineFront = false;
    bool leftEngineBack = false;
    bool rightEngineFront = false;
    bool rightEngineBack = false;
};

MovementModel movement_model;

const int model_table_size = 8;

MovementModel modelTable[model_table_size] = {
    MovementModel{true, false, false, false, false, false, false},
    MovementModel{true, true, false, true, false, false, true},
    MovementModel{true, false, true, false, true, true, false},
    MovementModel{true, true, true, true, false, true, false},
    MovementModel{false, false, false, false, false, false, false},
    MovementModel{false, true, false, false, true, true, false},
    MovementModel{false, false, true, true, false, false, true},
    MovementModel{false, true, true, false, true, false, true}
};

void updateMovement(bool isForwards, bool leftActive, bool rightActive){
    movement_model.isForwards = isForwards;
    movement_model.leftActive = leftActive;
    movement_model.rightActive = rightActive;

    for (auto matchedModel : modelTable)
    {
        if (matchedModel.isForwards == movement_model.isForwards && matchedModel.leftActive == movement_model.leftActive && matchedModel.rightActive == movement_model.rightActive)
        {
            movement_model = matchedModel;
            break;
        }
    }
}

void updateMovement(DirectionsEnum direction){
    bool leftActive = direction == DirectionsEnum::left || direction == DirectionsEnum::straight;
    bool rightActive = direction == DirectionsEnum::right || direction == DirectionsEnum::straight;

    updateMovement(true, leftActive, rightActive);
}