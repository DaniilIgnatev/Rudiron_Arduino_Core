#include "Arduino.h"
#include "rudiron/tasks_timer.h"

void setup_pinout();
void setup_tasks();
void wellcome();

///Первая кнопка нажата
bool pressed1 = false;

///Вторая кнопка нажата
bool pressed2 = false;

///Третья кнопка нажата
bool pressed3 = false;

///Пример задачи моргания первым светодиодом
bool task_led1(void *)
{
    digitalWrite(LED_BUILTIN_1, !digitalRead(LED_BUILTIN_1));
    return true;
}

///Пример задачи моргания вторым светодиодом
bool task_led2(void *)
{
    digitalWrite(LED_BUILTIN_2, !digitalRead(LED_BUILTIN_2));
    return true;
}

///Код выполняется один раз
void setup()
{
    setup_pinout();
    setup_tasks();
    wellcome();
}

///Устанавливаются режимы ввода-вывода для используемых пинов
void setup_pinout()
{
    pinMode(BUTTON_BUILTIN_1, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_2, INPUT_PULLDOWN);
    pinMode(BUTTON_BUILTIN_3, INPUT_PULLDOWN);

    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);
}

///Настривается расписание вызовов функций
void setup_tasks()
{
    tasksTimer.start_every_millis(1000, task_led1);
    tasksTimer.start_every_millis(2000, task_led2);
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

///Код выполняется многократно
void loop()
{
    pressed1 = digitalRead(BUTTON_BUILTIN_1);
    pressed2 = digitalRead(BUTTON_BUILTIN_2);
    pressed3 = digitalRead(BUTTON_BUILTIN_3);
}
