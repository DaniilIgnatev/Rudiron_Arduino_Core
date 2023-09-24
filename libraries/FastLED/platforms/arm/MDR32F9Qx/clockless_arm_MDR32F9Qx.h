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

#ifndef __INC_CLOCKLESS_ARM_MDR32F9Qx_H
#define __INC_CLOCKLESS_ARM_MDR32F9Qx_H

#include "rudiron/timer.h"

FASTLED_NAMESPACE_BEGIN

#define FASTLED_HAS_CLOCKLESS 1

#define BUF_DMA_LENGTH 24

#define BitIsSet(reg, bit) ((reg & (1 << bit)) != 0)

template <int DATA_PIN, int T1, int T2, int T3, EOrder RGB_ORDER = RGB, int XTRA0 = 0, bool FLIP = false, int WAIT_TIME = 50>
class ClocklessController : public CPixelLEDController<RGB_ORDER>
{
protected:
    Rudiron::PortPinName pinName;

    uint16_t BUF_DMA_FIRST[BUF_DMA_LENGTH];

    uint16_t BUF_DMA_SECOND[BUF_DMA_LENGTH];

    uint16_t *BUF_DMA;

    uint16_t PWM_LOW;

    uint16_t PWM_HIGH;

    static void DMA_Interrupt_Handler(Rudiron::Timer &timer)
    {
        timer.PWM_DMA_wait_done(); // без ожидания происходит рассинхронизация
        timer.disable();           // без остановки таймера, шим продолжает работу
    }

public:
    virtual void init()
    {
        // Serial.begin(115200);
        this->pinName = Rudiron::GPIO::get_rudiron_gpio(DATA_PIN);
        Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_pinName(this->pinName);
        uint32_t frequency = 1000000000 / (T1 + T2 + T3);
        // Serial.print("Frequency = ");
        // Serial.println(frequency);

        timer.setup(frequency);

        uint16_t ARR = timer.get_ARR();
        // Serial.print("ARR = ");
        // Serial.println(ARR);

        PWM_LOW = ARR / 3 - 1;
        // PWM_LOW = (T1) * (ARR + 1) / (T1 + T2 + T3) + 1; //должно быть так, но dma выдает артефакты

        // Serial.print("PWM_LOW = ");
        // Serial.println(PWM_LOW);

        PWM_HIGH = ARR - PWM_LOW - 1;
        // PWM_HIGH = (T1 + T2) * (ARR + 1) / (T1 + T2 + T3) + 1; //должно быть так, но dma выдает артефакты

        // Serial.print("PWM_HIGH = ");
        // Serial.println(PWM_HIGH);

        timer.PWM_DMA_setup(pinName, BUF_DMA_FIRST, BUF_DMA_LENGTH, &DMA_Interrupt_Handler);
    }

    virtual uint16_t getMaxRefreshRate() const { return 400; }

protected:
    virtual void showPixels(PixelController<RGB_ORDER> &pixels)
    {
        uint8_t b1, b2, b3;

        if (Rudiron::CLK::getSpeed() == Rudiron::CLK_Speed::low)
        {
            Rudiron::CLK::pause_delay();
        }

        BUF_DMA = BUF_DMA_FIRST;

        while (pixels.has(1))
        {
            pixels.stepDithering();

            b1 = pixels.loadAndScale0();
            b2 = pixels.loadAndScale1();
            b3 = pixels.loadAndScale2();

            for (int i = 0; i < 8; i++)
            {
                if (BitIsSet(b1, (7 - i)) == 1)
                {
                    BUF_DMA[i + 0] = PWM_HIGH;
                }
                else
                {
                    BUF_DMA[i + 0] = PWM_LOW;
                }

                if (BitIsSet(b2, (7 - i)) == 1)
                {
                    BUF_DMA[i + 8] = PWM_HIGH;
                }
                else
                {
                    BUF_DMA[i + 8] = PWM_LOW;
                }

                if (BitIsSet(b3, (7 - i)) == 1)
                {
                    BUF_DMA[i + 16] = PWM_HIGH;
                }
                else
                {
                    BUF_DMA[i + 16] = PWM_LOW;
                }
            }

            Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_pinName(this->pinName);
            timer.PWM_DMA_update(BUF_DMA, BUF_DMA_LENGTH);
            BUF_DMA = (BUF_DMA == BUF_DMA_FIRST) ? BUF_DMA_SECOND : BUF_DMA_FIRST;

            pixels.advanceData();
        }

        if (Rudiron::CLK::getSpeed() == Rudiron::CLK_Speed::low)
        {
            Rudiron::CLK::resume_delay();
        }
    }
};

FASTLED_NAMESPACE_END

#endif
