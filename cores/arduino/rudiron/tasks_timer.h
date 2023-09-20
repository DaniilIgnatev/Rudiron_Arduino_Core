/*
This file is part of Arduino_Core_Rudiron.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron.  If not, see <https://www.gnu.org/licenses/>.

The original idea and code was taken from arduino-timer_name library (regards to the author), then heavily optimized, simplified and tuned in the sdk to improve the user experience.
https://www.arduino.cc/reference/en/libraries/arduino-timer/
*/

#pragma once

#include "clk.h"
#include <limits.h>

namespace Rudiron
{
#define TASKS_TIMER_MAX_TASKS 8

    /// Размещает и отслеживает задачи, вызывая их обработики при наступлении указанного момента времени
    class TasksTimer
    {
    public:
        /// Тип данных ID задачи
        typedef uintptr_t TaskID;

        /// Тип данных функции-обработчика
        typedef bool (*handler_t)(void *opaque);

        /// Вызывает обработчик в определенный момент времени (микросекунды)
        TaskID start_at_micros(unsigned long time, handler_t handler, void *argument = nullptr)
        {
            const unsigned long now = Rudiron::CLK::micros();
            return task_id(add_task(now, time - now, handler, argument));
        }

        /// Вызывает обработчик в определенный момент времени (миллисекунды)
        inline TaskID start_at_millis(unsigned long time, handler_t handler, void *argument = nullptr)
        {
            return start_at_micros(time * 1000, handler, argument);
        }

        /// Вызывает обработчик через промежуток времени (микросекунды)
        TaskID start_in_micros(unsigned long delay, handler_t handler, void *argument = nullptr)
        {
            return task_id(add_task(Rudiron::CLK::micros(), delay, handler, argument));
        }

        /// Вызывает обработчик через промежуток времени (миллисекунды)
        inline TaskID start_in_millis(unsigned long delay, handler_t handler, void *argument = nullptr)
        {
            return start_in_micros(delay * 1000, handler, argument);
        }

        /// Вызывает обработчик через каждый промежуток времени (микросекунды)
        TaskID start_every_micros(unsigned long interval, handler_t handler, void *argument = nullptr)
        {
            return task_id(add_task(Rudiron::CLK::micros(), interval, handler, argument, interval));
        }

        /// Вызывает обработчик через каждый промежуток времени (миллисекунды)
        inline TaskID start_every_millis(unsigned long interval, handler_t handler, void *argument = nullptr)
        {
            return start_every_micros(interval * 1000, handler, argument);
        }

        /// Отменяет указанный обработчик
        void cancel(TaskID &task)
        {
            if (!task)
            {
                return;
            }

            for (int i = 0; i < TASKS_TIMER_MAX_TASKS; i++)
            {
                if (tasks[i].handler && task_id(&tasks[i]) == task)
                {
                    remove(&tasks[i]);
                    break;
                }
            }

            task = static_cast<TaskID>(NULL);
        }

        /// Отменяет все обработчики
        void cancelAll()
        {
            for (int i = 0; i < TASKS_TIMER_MAX_TASKS; i++)
            {
                remove(&tasks[i]);
            }
        }

        /// Проверяет обработчики на необходимость передачи им управления
        void tick()
        {
            const unsigned long t = Rudiron::CLK::micros();

            for (int i = 0; i < TASKS_TIMER_MAX_TASKS; i++)
            {
                if (tasks[i].handler)
                {
                    const unsigned long duration = t - tasks[i].start;

                    if (duration >= tasks[i].expires)
                    {
                        tasks[i].repeat = tasks[i].handler(tasks[i].opaque) && tasks[i].repeat;

                        if (tasks[i].repeat)
                        {
                            tasks[i].start = t;
                        }
                        else
                        {
                            remove(&tasks[i]);
                        }
                    }
                }
            }
        }

        /// Количество размещенных обработчиков
        size_t count() const
        {
            return tasks_count;
        }

        /// Проверка на отсутствие обработчиков
        inline bool empty() const
        {
            return tasks_count == 0;
        }

    private:
        int tasks_count = 0; // Количество размещенных задач
        size_t ctr = 0;      // Идентификатор

        /// Дескриптор задачи
        struct Task
        {
            handler_t handler; // Функция-обработчик
            void *opaque;      // Аргумент обработчику
            uint64_t start;    // Время запуска задачи
            uint64_t expires;  // Время завершения задачи
            bool repeat;       // Задача повторится
            size_t id;         // Идентификатор
        };

        Task tasks[TASKS_TIMER_MAX_TASKS];

        void remove(Task *task)
        {
            task->handler = NULL;
            task->opaque = NULL;
            task->start = 0;
            task->expires = 0;
            task->repeat = 0;
            task->id = 0;

            tasks_count--;
        }

        inline TaskID task_id(const Task *const t)
        {
            const TaskID id = reinterpret_cast<TaskID>(t);

            return id ? id ^ t->id : id;
        }

        Task *next_task_slot()
        {
            for (int i = 0; i < TASKS_TIMER_MAX_TASKS; i++)
            {
                if (tasks[i].handler == NULL)
                    return &tasks[i];
            }

            return NULL;
        }

        Task *add_task(
            unsigned long start,
            unsigned long expires,
            handler_t h, void *opaque,
            bool repeat = 0)
        {
            Task *const slot = next_task_slot();

            if (!slot)
            {
                return NULL;
            }

            tasks_count++;

            slot->id = ctr++;
            slot->handler = h;
            slot->opaque = opaque;
            slot->start = start;
            slot->expires = expires;
            slot->repeat = repeat;

            return slot;
        }
    };
}

#ifdef TASKS_TIMER_ENABLED
extern Rudiron::TasksTimer tasksTimer;
#endif