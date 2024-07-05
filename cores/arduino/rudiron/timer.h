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

#ifndef TIMER_H
#define TIMER_H

#include "timer_utility.h"
#include "MDR32F9Qx_dma.h"

namespace Rudiron
{
    /// Не использовать с переменными типа auto, будет ошибка
    class Timer
    {
    private:
        TimerName name;

        MDR_TIMER_TypeDef *MDR_TIMER;

        uint32_t RST_CLK_PCLK;

        IRQn TIMER_IRQn;

        uint16_t prescaler;

        uint16_t ARR;

        uint32_t frequency = 0;

    public:
        MDR_TIMER_TypeDef *get_MDR_TIMER() const;

        uint16_t get_ARR() const;

        explicit Timer(TimerName name);

        ~Timer();

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
        inline bool isHighFrequency();

    public:
        void setup(uint32_t frequency = 500);

    private:
        PORT_InitTypeDef PWM_getInitTypeDef(PortPinName pinName, PORT_FUNC_TypeDef func);

        void PWM_initPin(PortPinName pinName);

    private:
        int PWM_activateChannel(PortPinName pinName, uint16_t width, uint16_t width_max, bool withNegative, bool ignoreCompare = false);

    public:
        // Запустить генерацию ШИМ на выводе pinName и с заполнением width, между 0 и width_max
        void PWM_start(PortPinName pinName, uint16_t width, uint16_t width_max);

        // Запустить генерацию ШИМ на выводах pinName и invertedPinName и с заполнением width, между 0 и width_max
        void PWM_start(PortPinName pinName, PortPinName invertedPinName, uint16_t width, uint16_t width_max);

        // Остановить генерацию ШИМ на выводе pinName
        void PWM_stop(PortPinName pinName);

    private:
        DMA_ChannelInitTypeDef DMA_InitStr;
        DMA_CtrlDataInitTypeDef DMA_PriCtrlStr;

        uint8_t DMA_Channel;

    public:
        // Возвращает канал DMA, соответствующий данному таймеру
        uint8_t getDmaChannel() const;

    private:
        // Пользовательский обработчик прерывания DMA, вызыванный (CNT == ARR) и разрешенным DMA
        void (*dma_interrupt_handler)(Timer &timer) = nullptr;

        // Глобальный обработчик прерывания, вызыванный (CNT == ARR) и разрешенным DMA
        static void DMA_Interrupt_Handler_1();

        // Глобальный обработчик прерывания, вызыванный (CNT == ARR) и разрешенным DMA
        static void DMA_Interrupt_Handler_2();

        // Глобальный обработчик прерывания, вызыванный (CNT == ARR) и разрешенным DMA
        static void DMA_Interrupt_Handler_3();

        void PWM_DMA_init(uint32_t channel_number, uint16_t *buffer, uint16_t buffer_length,
                          void (*dma_interrupt_handler)(Timer &timer));

    public:
        // Настроить и запустить DMA с заданными параметрами
        // pinName - вывод GPIO для ШИМ
        // buffer - источник данных
        // buffer_length - длина источника данных
        // dma_interrupt_handler - обработчик, вызываемый после каждого полного периода таймера
        void PWM_DMA_setup(PortPinName pinName,
                           uint16_t *buffer,
                           uint16_t buffer_length,
                           void (*dma_interrupt_handler)(Timer &timer));

        // Настроить и запустить DMA с заданными параметрами
        // pinName - вывод GPIO для ШИМ
        // invertedPinName - вывод GPIO для инвертированного ШИМ
        // buffer - источник данных
        // buffer_length - длина источника данных
        // dma_interrupt_handler - обработчик, вызываемый после каждого полного периода таймера
        void PWM_DMA_setup(PortPinName pinName,
                           PortPinName invertedPinName,
                           uint16_t *buffer,
                           uint16_t buffer_length,
                           void (*dma_interrupt_handler)(Timer &timer));

        bool PWM_DMA_done();

        void PWM_DMA_wait_done();

        void PWM_DMA_repeat();

        void PWM_DMA_update(uint16_t *buffer, uint16_t buffer_length);

        // Останавливает цикл DMA
        void PWM_DMA_stop();

    public:
        static TimerName getTimerNameForPin(PortPinName pinName);

        static Timer &getTimer_by_name(TimerName name);
        
        static bool hasTimer_for_pinName(PortPinName pinName);

        static Timer &getTimer_by_pinName(PortPinName name);

        static Timer &getTimer1();

        static Timer &getTimer2();

        static Timer &getTimer3();
    };

    void pwm(uint8_t pin, uint32_t frequency, uint16_t width, uint16_t width_max);
}

#endif // TIMER_H
