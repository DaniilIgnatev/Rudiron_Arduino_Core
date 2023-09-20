#ifndef ADC_TYPES_H
#define ADC_TYPES_H

#include "MDR32F9Qx_adc.h"
#include "MDR32Fx.h"

namespace Rudiron
{
    typedef enum
    {
        ADC_None = -1,
        ADC_1 = 0,
        ADC_2,
    } ADC_Name;

    // Канал АЦП
    typedef enum
    {
        ADC_Channel_None = -1,
        ADC_Channel_0 = 0,
        ADC_Channel_1,
        ADC_Channel_2,
        ADC_Channel_3,
        ADC_Channel_4,
        ADC_Channel_5,
        ADC_Channel_6,
        ADC_Channel_7,
        ADC_Channel_8,
        ADC_Channel_9,
        ADC_Channel_10,
        ADC_Channel_11,
        ADC_Channel_12,
        ADC_Channel_13,
        ADC_Channel_14,
        ADC_Channel_15,
        ADC_Channel_16,
        ADC_Channel_17,
        ADC_Channel_18,
        ADC_Channel_19,
        ADC_Channel_20,
        ADC_Channel_21,
        ADC_Channel_22,
        ADC_Channel_23,
        ADC_Channel_24,
        ADC_Channel_25,
        ADC_Channel_26,
        ADC_Channel_27,
        ADC_Channel_28,
        ADC_Channel_29,
        ADC_Channel_30,
        ADC_Channel_31,
    } ADC_ChannelName;

    // Результат работы АЦП
    struct ADCResult
    {
        bool valid;              // является действительным значением
        bool override;           // было перезаписано
        uint16_t value;          // 0-4095
        ADC_ChannelName channel; // номер канала
    };
}

#endif // ADC_TYPES_H