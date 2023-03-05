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
*/

#ifndef TIMER_H
#define TIMER_H

#include "timerutility.h"
#include "MDR_dma.h"

namespace Rudiron
{
    class Timer
    {
    public:
        static bool hasTimerForPin(PortPinName pinName);

        static TimerName getTimerNameForPin(PortPinName pinName);

    protected:
        TimerName name;

        MDR_TIMER_TypeDef *MDR_TIMER;

        uint32_t RST_CLK_PCLK;

        IRQn TIMER_IRQn;

        uint16_t prescaler;

        uint16_t ARR;

        uint32_t frequency = 0;

    public:
        inline uint16_t getARR(){
            return ARR;
        }

        explicit Timer(TimerName name);

        inline void start(){
            TIMER_Cmd(this->MDR_TIMER, ENABLE);
            NVIC_EnableIRQ(TIMER_IRQn);
        }

        inline void stop(){
            NVIC_DisableIRQ(TIMER_IRQn);
            TIMER_Cmd(this->MDR_TIMER, DISABLE);
        }

    protected:
        inline bool isHighFrequency();

        PORT_InitTypeDef PWM_getInitTypeDef(PortPinName pinName, PORT_FUNC_TypeDef func);

        void PWM_initPin(PortPinName pinName);

        int PWM_activateChannel(PortPinName pinName, uint16_t ppm, bool withNegative, bool ignoreCompare = false);

    public:
        void PWM_setup(uint32_t frequency = 500);

        void PWM_start(PortPinName pinName, uint16_t ppm);

        void PWM_start(PortPinName pinName, PortPinName invertedPinName, uint16_t ppm);

        void PWM_stop(PortPinName pinName);

    protected:
        DMA_ChannelInitTypeDef DMA_InitStr;
        DMA_CtrlDataInitTypeDef DMA_PriCtrlStr;

        uint32_t DMA_TIMER_CHANNEL;

        uint8_t DMA_Channel;

        uint8_t *BUF_DMA;
        uint16_t BUF_DMA_LENGTH;

        void PWM_DMA_init(uint32_t channel_number, uint16_t *buffer, uint16_t buffer_length);

    public:
        void PWM_DMA_setup(PortPinName pinName, uint16_t *buffer, uint16_t buffer_length);

        void PWM_DMA_setup(PortPinName pinName, PortPinName invertedPinName, uint16_t *buffer, uint16_t buffer_length);

        void PWM_DMA_setBuffer(uint16_t *buffer, uint16_t buffer_length);

        void PWM_DMA_start_single();

        void PWM_DMA_start_single(uint16_t *buffer, uint16_t buffer_length);

        void PWM_DMA_stop();
    public:
        static Timer *getTimerForPinName(PortPinName name);

        static Timer *getTimer1();

        static Timer *getTimer2();

        static Timer *getTimer3();
    };
}

#endif // TIMER_H
