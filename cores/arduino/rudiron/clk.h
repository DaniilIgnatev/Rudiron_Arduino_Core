/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron_MDR32F9Qx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron_MDR32F9Qx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron_MDR32F9Qx. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
*/

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#ifndef CLK_H
#define CLK_H

#include "config.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_eeprom.h"

namespace Rudiron
{
    enum CLK_Speed
    {
        low,
        medium,
        high
    };

    extern CLK_Speed _CLK_Speed;

    extern uint32_t _CPU_PLLmul;

    extern uint32_t _HCLKdiv;

    class CLK
    {
    private:
        /// Получить дескриптор тактирования
        static inline RST_CLK_FreqTypeDef getCLKDescriptor()
        {
            RST_CLK_FreqTypeDef RST_CLK_Clocks;
            RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
            return RST_CLK_Clocks;
        }

        static void updateHighSpeedConfig();

        /// Запуск на частоте внешнего генератора
        static void runHSE(uint32_t RST_CLK_CPU_PLLmul);

        /// Запуск на частоте внутреннего генератора
        static void runHSI(uint32_t RST_CLK_CPU_PLLmul);

        /// Установление задержки памяти
        static void updateEEPROMLatency(bool external, uint32_t RST_CLK_CPU_PLLmul);

        /// Инициализация системного таймера
        static void init_delay();

    public:
        static void pause_delay();

        static void resume_delay();

    public:
        static inline void initialize()
        {
            updateHighSpeedConfig();
            setCPUSpeed(CLK_Speed::high);
            enable_ADC_clock();
        }

        /// Копия значения _CLK_Speed
        static inline CLK_Speed getSpeed()
        {
            return _CLK_Speed;
        }

        /// Установка частоты микропроцессора
        static void setCPUSpeed(CLK_Speed newValue);

        /// Частота микропроцессора в герцах
        static inline uint32_t getCPUFrequency()
        {
            return getCLKDescriptor().CPU_CLK_Frequency;
        }

        /// Коэффициент умножения тактовой частоты микропроцессора
        static inline uint32_t getCPU_PLLmul()
        {
            return _CPU_PLLmul;
        }

        /// Коэффициент деления тактирования блоков микроконтроллера
        static inline uint32_t getHCLKdiv()
        {
            return _HCLKdiv;
        }

        /// Во сколько раз установленная частота процессора выше базовой
        static inline uint8_t getCPU_Multiplier()
        {
            return 1 << _HCLKdiv;
        }

        /// Копия значения _millis
        static inline uint32_t millis(void)
        {
            return _micros / 1000;
        }

        /// Копия значения _micros
        static inline uint32_t micros(void)
        {
            return _micros;
        }

        /// Выставление задержки в миллисекундах
        static void delay_millis(uint32_t ms);

        /// Выставление задержки в микросекундах
        static void delay_micros(uint32_t us);

        /// Активация тактовой частоты АЦП
        static void enable_ADC_clock();
    };
}

#endif // PORT_H
