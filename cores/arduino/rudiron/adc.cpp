#include "adc.h"

#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_adc.h"
#include "MDR32F9Qx_dma.h"
#include "MDR32F9Qx_config.h"
#include "clk.h"

namespace Rudiron
{
    ADC::ADC(ADC_Name adc_name)
    {
        this->adc_name = adc_name;
    }

    ADC::~ADC()
    {
        ADC::disable_unit();
    }

    void ADC::enable_unit()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);
    }

    void ADC::disable_unit()
    {
        NVIC_DisableIRQ(ADC_IRQn);
        ADC_DeInit();
        RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, DISABLE);
    }

    void ADC::enable()
    {
        switch (adc_name)
        {
        case ADC_1:
            ADC1_Cmd(ENABLE);
            break;
        case ADC_2:
            ADC2_Cmd(ENABLE);
            break;
        default:
            // ошибка
            break;
        }
    }

    void ADC::disable()
    {
        switch (adc_name)
        {
        case ADC_1:
            ADC1_Cmd(DISABLE);
            break;
        case ADC_2:
            ADC2_Cmd(DISABLE);
            break;
        default:
            // ошибка
            break;
        }
    }

    void ADC::initPinADC(PortPinName pinName)
    {
        PORT_InitTypeDef PORT_InitStructure;
        uint16_t pin = GPIO::get_arduino_gpio(pinName);

        PORT_InitStructure.PORT_Pin = pin;
        PORT_InitStructure.PORT_OE = PORT_OE_IN;
        PORT_InitStructure.PORT_MODE = PORT_MODE_ANALOG;

        PORT_InitStructure.PORT_FUNC = ::PORT_FUNC_PORT;
        PORT_InitStructure.PORT_GFEN = ::PORT_GFEN_OFF;
        PORT_InitStructure.PORT_PD = ::PORT_PD_DRIVER;
        PORT_InitStructure.PORT_PD_SHM = ::PORT_PD_SHM_OFF;
        PORT_InitStructure.PORT_PULL_DOWN = ::PORT_PULL_DOWN_OFF;
        PORT_InitStructure.PORT_PULL_UP = ::PORT_PULL_UP_OFF;
        PORT_InitStructure.PORT_SPEED = ::PORT_SPEED_FAST;

        GPIO::configPin(pinName, PORT_InitStructure);
    }

    void ADC::start_single()
    {
        switch (this->adc_name)
        {
        case ADC_1:
            ADC1_Start();
            break;
        case ADC_2:
            ADC2_Start();
            break;
        default:
            // ошибка
            break;
        }
    }

    void ADC::init(const ADCx_InitTypeDef *ADCx_InitStruct)
    {
        switch (this->adc_name)
        {
        case ADC_1:
            ADC1_Init(ADCx_InitStruct);
            break;
        case ADC_2:
            ADC2_Init(ADCx_InitStruct);
            break;
        default:
            // ошибка
            break;
        }
    }

    uint32_t ADC::get_result_value()
    {
        switch (this->adc_name)
        {
        case ADC_1:
            return ADC1_GetResult();
        case ADC_2:
            return ADC2_GetResult();
        default:
            // ошибка
            return 0;
        }
    }

    FlagStatus ADC::get_overwrite_flag_status()
    {
        switch (this->adc_name)
        {
        case ADC_1:
            return ADC1_GetFlagStatus(ADC1_FLAG_OVERWRITE);
        case ADC_2:
            return ADC2_GetFlagStatus(ADC2_FLAG_OVERWRITE);
        default:
            // ошибка
            return FlagStatus::RESET;
        }
    }

    void ADC::clear_overwrite_flag()
    {
        switch (this->adc_name)
        {
        case ADC_1:
            ADC1_ClearOverwriteFlag();
            break;
        case ADC_2:
            ADC2_ClearOverwriteFlag();
            break;
        default:
            // ошибка
            break;
        }
    }

    void __attribute__((optimize("O0"))) ADC::setup_channel(ADC_ChannelName channel_name)
    {
        enable_unit();
        disable();

        ADC_DeInit();
        ADC_InitTypeDef sADC;
        ADC_StructInit(&sADC);
        sADC.ADC_TempSensor = ADC_TEMP_SENSOR_Enable;
        sADC.ADC_TempSensorAmplifier = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
        sADC.ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Enable;
        sADC.ADC_IntVRefConversion = ADC_VREF_CONVERSION_Enable;
        ADC_Init(&sADC);

        ADCx_InitTypeDef sADCx;
        ADCx_StructInit(&sADCx);
        sADCx.ADC_ClockSource = ADC_CLOCK_SOURCE_ADC;
        sADCx.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
        sADCx.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
        sADCx.ADC_ChannelNumber = (uint32_t)channel_name;
        sADCx.ADC_Channels = 0;
        sADCx.ADC_LevelControl = ADC_LEVEL_CONTROL_Disable;
        sADCx.ADC_LowLevel = 0;
        sADCx.ADC_HighLevel = 0;
        sADCx.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;
        sADCx.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;
        sADCx.ADC_Prescaler = ADC_CLK_div_None;
        sADCx.ADC_DelayGo = 7;
        init(&sADCx);
    }

    ADCResult __attribute__((optimize("O0"))) ADC::read_channel_single(ADC_ChannelName channel_name)
    {
        setup_channel(channel_name);

        enable();
        start_single();

        Rudiron::CLK::delay_micros(10); // пауза с расчетом на рабочую частоту АЦП 16Мгц

        int resultReg = get_result_value();

        ADCResult result;
        result.valid = false;
        result.channel = (ADC_ChannelName)(resultReg >> 16);
        result.value = (uint16_t)(resultReg & 0xFFF);
        result.override = (bool)get_overwrite_flag_status();
        result.valid = true;

        clear_overwrite_flag();
        disable_unit();

        return result;
    }

    ADCResult __attribute__((optimize("O0"))) ADC::read_pin_single(PortPinName pinName)
    {
        ADCResult result;
        result.valid = false;

        PortName portName = GPIO::getPortName(pinName);
        if (portName == PORT_D)
        {
            ADC::initPinADC(pinName);

            ADC_ChannelName channelName = ADC_Utility::getChannelName(pinName);
            result = read_channel_single(channelName);
        }

        return result;
    }

    ADCResult ADC::read_internal_reference_voltage_source()
    {
        ADCResult result;
        result.valid = false;

        if (adc_name == ADC_1)
        {
            ADC_ChannelName channelName = ADC_ChannelName::ADC_Channel_30;
            result = read_channel_single(channelName);
        }

        return result;
    }

    ADCResult ADC::read_temperature_sensor()
    {
        ADCResult result;
        result.valid = false;

        if (adc_name == ADC_1)
        {
            ADC_ChannelName channelName = ADC_ChannelName::ADC_Channel_31;
            result = read_channel_single(channelName);
        }

        return result;
    }

#define ADC_TS_T1 25.0F // Температура в 1-й точке
#define ADC_TS_D1 0x6A0 // Значение АЦП в 1-й точке
#define ADC_TS_T2 60.0F // Температура во 2-й точке
#define ADC_TS_D2 0x7A0 // Значение АЦП во 2-й точке

    float ADC::temperature_raw_to_celsius(ADCResult raw, float Vref)
    {
        const float raw_value = raw.value;

        // const float V25 = 0.76;
        // const float Avg_Slope = 0.0025;
        // volatile float temperature = ((raw_value / (float)4096.0) * Vref - V25) / Avg_Slope + (float)25.0;

        const float temperature = (float)((raw_value - ADC_TS_D1) * (ADC_TS_T2 - ADC_TS_T1)) / (float)(ADC_TS_D2 - ADC_TS_D1) + ADC_TS_T1;
        return temperature;
    }

    // Возвращает класс АЦП №1
    ADC &ADC::getADC1()
    {
        static ADC adc = ADC(ADC_Name::ADC_1);
        return adc;
    }

    // Возвращает класс АЦП №2
    ADC &ADC::getADC2()
    {
        static ADC adc = ADC(ADC_Name::ADC_2);
        return adc;
    }
}
