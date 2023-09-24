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

#ifndef DAC_H
#define DAC_H

#include "dac_utility.h"
#include "gpio.h"
#include "dma_types.h"
#include "rudiron/timer.h"

namespace Rudiron
{
    /// Не использовать с переменными типа auto, будет ошибка
    class DAC
    {
    private:
        DAC_Name dac_name;

    public:
        DAC(DAC_Name dac_name);

        ~DAC();

    public:
        // Включает тактирование
        void enable_unit();

        // Отключает тактирование
        void disable_unit();

        // Разрешает работу
        void enable();

        // Запрещает работу
        void disable();

    private:
        uint32_t getValue();

        void setValue(uint32_t value);

    private:
        static uint32_t DAC1_Ref;

        static uint32_t DAC2_Ref;

        static void initPinDAC_Ref(PortPinName pinName);

    public:
        // Задать внешний или внутренний источник опорного напряжения
        void set_reference(bool external);

    private:
        static void initPinDAC_Data(PortPinName pinName);

        bool activate_pin(PortPinName pinName);

    public:
        // Выставляет и удерживает на выводе pinName уровень напряжения (0-3.3V), соответствующий значению value 12-бит (0-4095)
        void write_pin_single(PortPinName pinName, uint32_t value);

    private:
        TimerName timer_name = Timer_None;
        uint8_t dma_channel;

        DMA_ChannelInitTypeDef DMA_InitStr;
        DMA_CtrlDataInitTypeDef DMA_PriCtrlStr;
        DMA_CtrlDataInitTypeDef DMA_AltCtrlStr;

    private:
        // Пользовательский обработчик прерывания DMA
        void (*dma_interrupt_handler)(DAC &dac) = nullptr;

        static void DMA_Interrupt_Handler();

        const uint32_t get_MDR_DAC_Data_Address();

    public:
        // Настроить и запустить DMA с заданными параметрами
        // timer_name - задействованный таймер
        // frequency - частота таймера и задания выходного сигнала ЦАП
        // pinName - вывод GPIO для ЦАП
        // buffer_primary - основной источник данных
        // buffer_primary_length - длина основного источника данных
        // dma_interrupt_handler - обработчик, вызываемый после каждого изменения данных ЦАП контроллером DMA
        // primary_transfers - число последовательных проходов по основной структуре данных
        // alternate_buffer - задействовать ли альтернативную структуру данных
        // buffer_alternate - альтернативный источник данных
        // buffer_alternate_length - длина альтернативного источника данных
        // alternate_transfers - число последовательных проходов по альтернативной структуре данных
        void DMA_setup(TimerName timer_name,
                       uint32_t frequency,
                       PortPinName pinName,
                       uint16_t *buffer_primary,
                       uint16_t buffer_primary_length,
                       void (*dma_interrupt_handler)(DAC &dac) = nullptr,
                       DMA_Number_Continuous_Transfers primary_transfers = DMA_Number_Continuous_Transfers_1,
                       bool alternate_buffer = false,
                       uint16_t *buffer_alternate = nullptr,
                       uint16_t buffer_alternate_length = 0,
                       DMA_Number_Continuous_Transfers alternate_transfers = DMA_Number_Continuous_Transfers_1);

        // Повторно запустить DMA с ранее выставленными настройками
        void DMA_repeat();

        // Изменить источник данных для основной или альтернативной структуры данных DMA
        void DMA_update(uint16_t *buffer, uint16_t buffer_length, bool primary_control_data = true);

        // Возвращает, завершил ли DMA работу
        bool DMA_done();

        // Синхронное ожидание завершения работы DMA
        void DMA_wait_done();

        // Возвращает статус, обрабатывает ли DMA в настоящий момент альтернативную управляющую структуру
        bool DMA_is_alternate_active();

        // Останавливает цикл DMA
        void DMA_stop();

    public:
        // Возвращает класс ЦАП №1
        static DAC &getDAC1();

        // Возвращает класс ЦАП №2
        static DAC &getDAC2();
    };
}

#endif // DAC_H
