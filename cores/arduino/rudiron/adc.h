/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
*/

#ifndef ADC_H
#define ADC_H

#include "adc_utility.h"
#include "gpio.h"

namespace Rudiron
{
    /// Не использовать с переменными типа auto, будет ошибка
    class ADC
    {
    private:
        ADC_Name adc_name;

    public:
        ADC(ADC_Name adc_name);

        ~ADC();

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
        static void initPinADC(PortPinName pinName);

        void start_single();

        void init(const ADCx_InitTypeDef *ADCx_InitStruct);

        uint32_t get_result_value();

        FlagStatus get_overwrite_flag_status();

        void clear_overwrite_flag();

        void setup_channel(ADC_ChannelName channel_name);

        ADCResult read_channel_single(ADC_ChannelName channel_name);

    public:
        // Возвращает значение, считанное с внешнего вывода pinName
        ADCResult read_pin_single(PortPinName pinName);

        // Возвращает значение, считанное с внутреннего источника опорного напряжения. Работает только для первого АЦП.
        ADCResult read_internal_reference_voltage_source();

        // Возвращает значение, считанное с встроенного датчика температуры. Работает только для первого АЦП.
        ADCResult read_temperature_sensor();

        // Преобразование необработанного значения в температуру в градусах Цельсия
        // raw - необработанное значение с ацп
        // Vref - опорное напряжение в вольтах
        static float temperature_raw_to_celsius(ADCResult raw, float Vref = 3.3);

    public:
        // Возвращает класс АЦП №1
        static ADC &getADC1();

        // Возвращает класс АЦП №2
        static ADC &getADC2();
    };
}

#endif // ADC_H
