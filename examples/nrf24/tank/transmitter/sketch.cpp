#include "Arduino.h"
#include "rudiron/nrf24.h"

using namespace Rudiron;

///Первая кнопка нажата
bool pressed1 = false;

///Вторая кнопка нажата
bool pressed2 = false;

///Третья кнопка нажата
bool pressed3 = false;

uint8_t package[3];

///Устанавливаются режимы ввода-вывода для используемых пинов
void setup_pinout()
{
    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);
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

void setup(){
    setup_pinout();
    wellcome();

    if (!nrf24.begin(false)){
        Serial.println("Nrf24 Error!");
        return;
    }
}

bool pressed2_last = false;
bool isForwards = false;

void loop(){
    package[1] = pressed1;
    package[2] = pressed3;

    if (!pressed2_last && pressed2){
        isForwards = !isForwards;
        digitalWrite(LED_BUILTIN_1, !isForwards);
        digitalWrite(LED_BUILTIN_2, isForwards);
    }
    pressed2_last = pressed2;
    package[0] = isForwards;
    
    if (nrf24.availableForWrite()){
        nrf24.write(package, 3);
    }

    delay(100);
}
