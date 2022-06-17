/*
This file is part of Rudiron_Arduino_Core.

Rudiron_Arduino_Core is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Rudiron_Arduino_Core is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Rudiron_Arduino_Core.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CLK_H
#define CLK_H

#include "MDR_config.h"
#include "MDR32Fx.h"
#include "MDR_port.h"
#include "MDR_rst_clk.h"
#include "MDR_eeprom.h"

namespace Rudiron
{
    enum CLK_Speed
    {
        low,
        medium,
        high
    };

    extern CLK_Speed _CLK_Speed;

    extern uint32_t _CPU_Multiplier;

    extern uint32_t _HCLKdiv;

    class CLK
    {
    private:
        ///Получить дескриптор тактирования
        static inline RST_CLK_FreqTypeDef getCLKDescriptor()
        {
            RST_CLK_FreqTypeDef RST_CLK_Clocks;
            RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
            return RST_CLK_Clocks;
        }

        ///Инициализация векторов прерываний
        static void init_irq();

        ///Установление задержки памяти
        static void updateEEPROMLatency(bool external, uint32_t RST_CLK_CPU_PLLmul);

        ///Запуск на частоте внешнего генератора
        static void runHSE(uint32_t RST_CLK_CPU_PLLmul);

        ///Запуск на частоте внутреннего генератора
        static void runHSI(uint32_t RST_CLK_CPU_PLLmul);

        ///Инициализация системного таймера
        static void init_delay();

    public:

        ///Копия значения _CLK_Speed
        static inline CLK_Speed getSpeed()
        {
            return _CLK_Speed;
        }

        ///Установка частоты микропроцессора
        static void setCPUSpeed(CLK_Speed newValue = CLK_Speed::low);

        ///Частота микропроцессора в герцах
        static inline uint32_t getCPUFrequency()
        {
            return getCLKDescriptor().CPU_CLK_Frequency;
        }

        ///Коэффициент умножения тактовой частоты микропроцессора
        static inline uint32_t getCPU_Multiplier();

        ///Коэффициент деления тактирования блоков микроконтроллера
        static uint32_t getHCLKdiv();

        ///Копия значения _millis
        static inline uint32_t millis(void)
        {
            return _micros / 1000;
        }

        ///Копия значения _micros
        static inline uint32_t micros(void)
        {
            return _micros;
        }

        ///Выставление задержки в миллисекундах
        static void delay_millis(uint32_t ms);

        ///Выставление задержки в микросекундах
        static void delay_micros(uint32_t us);
    };
}

#endif // PORT_H
