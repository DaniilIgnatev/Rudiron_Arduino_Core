#include "sketch.h"

#define leftEngineFrontPin 12
#define leftEngineBackPin 13
#define rightEngineFrontPin 14
#define rightEngineBackPin 15

using namespace Rudiron;

void setup_movement()
{
    pinMode(leftEngineFrontPin, OUTPUT);
    pinMode(leftEngineBackPin, OUTPUT);
    pinMode(rightEngineFrontPin, OUTPUT);
    pinMode(rightEngineBackPin, OUTPUT);
}

///Устанавливаются режимы ввода-вывода для используемых пинов
void setup_pinout()
{
    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    setup_movement();
}

///Приветствие пользователя
void wellcome()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED_BUILTIN_1, true);
        digitalWrite(LED_BUILTIN_2, true);
        delay(100);
        digitalWrite(LED_BUILTIN_1, false);
        digitalWrite(LED_BUILTIN_2, false);
        delay(100);
    }

    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");
}

void setup()
{
    setup_pinout();
    wellcome();

    if (!nrf24.begin(true))
    {
        Serial.println("Nrf24 Error!");
        return;
    }
}

struct TankMovementModel
{
    bool isForwards = false;
    bool leftActive = false;
    bool rightActive = false;

    bool leftEngineFront = false;
    bool leftEngineBack = false;
    bool rightEngineFront = false;
    bool rightEngineBack = false;
};

TankMovementModel model;

const int model_table_size = 8;

TankMovementModel modelTable[model_table_size] = {
    TankMovementModel{true, false, false, false, false, false, false},
    TankMovementModel{true, true, false, true, false, false, true},
    TankMovementModel{true, false, true, false, true, true, false},
    TankMovementModel{true, true, true, true, false, true, false},
    TankMovementModel{false, false, false, false, false, false, false},
    TankMovementModel{false, true, false, false, true, true, false},
    TankMovementModel{false, false, true, true, false, false, true},
    TankMovementModel{false, true, true, false, true, false, true}};

void updateModelInputs()
{
    model.isForwards = nrf24.read();
    model.leftActive = nrf24.read();
    model.rightActive = nrf24.read();
}

void updateModelOutputs()
{
    for (auto engineTruthTable : modelTable)
    {
        if (engineTruthTable.isForwards == model.isForwards && engineTruthTable.leftActive == model.leftActive && engineTruthTable.rightActive == model.rightActive)
        {
            model = engineTruthTable;
            break;
        }
    }
}

void move()
{
    updateModelOutputs();

    digitalWrite(leftEngineFrontPin, model.leftEngineFront);
    digitalWrite(rightEngineFrontPin, model.rightEngineFront);
    digitalWrite(leftEngineBackPin, model.leftEngineBack);
    digitalWrite(rightEngineBackPin, model.rightEngineBack);

    digitalWrite(LED_BUILTIN_1, model.leftActive);
    digitalWrite(LED_BUILTIN_2, model.rightActive);
}

void loop()
{
    if (nrf24.available())
    {
        updateModelInputs();
        move();
    }

    delay(50);
}
