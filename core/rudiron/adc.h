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

#ifndef ADC_H
#define ADC_H

#include "MDR_config.h"
#include "MDR32Fx.h"
#include "MDR_port.h"
#include "MDR_rst_clk.h"
#include "MDR_adc.h"
#include "MDR_dma.h"
#include "MDR_config.h"
#include "gpio.h"

namespace Rudiron
{
    class ADC
    {
    private:
        static void initPinADC(PortPinName pinName);

        static void enable();

        static void disable();

        static ADCResult lastResult;

    public:
        // Возвращает последнее значение, считанное с внешнего вывода pinName
        static ADCResult readPin(PortPinName pinName);
    };
}

#endif // ADC_H
