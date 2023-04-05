#include "adc.h"
#include "gpio.h"
#include "clk.h"

namespace Rudiron
{
    void ADC::enable()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);
        ADC1_ITConfig((ADCx_IT_OUT_OF_RANGE), DISABLE);
        ADC1_ITConfig((ADCx_IT_END_OF_CONVERSION), DISABLE);
        NVIC_EnableIRQ(ADC_IRQn);
        ADC1_Cmd(ENABLE);
    }

    void ADC::disable()
    {
        ADC1_Cmd(DISABLE);
        NVIC_DisableIRQ(ADC_IRQn);
        RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, DISABLE);
    }

    void ADC::initPinADC(PortPinName pinName)
    {
        PORT_InitTypeDef PORT_InitStructure;
        uint16_t pin = GPIO::getPinNumber(pinName);

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

    ADCResult ADC::lastResult;

    ADCResult ADC::read_pin(PortPinName pinName)
    {
        ADCResult result;
        result.valid = false;

        PortName portName = GPIO::getPortName(pinName);
        if (portName != PORT_D)
        {
            return result;
        }

        ADCChannelName channelName = (ADCChannelName)(pinName - PORT_PIN_D0);
        ADC::initPinADC(pinName);

        enable();

        ADC1_Cmd(DISABLE);
        ADC_DeInit();
        ADC_InitTypeDef sADC;
        ADC_StructInit(&sADC);
        ADC_Init(&sADC);

        ADCx_InitTypeDef sADCx;
        ADCx_StructInit(&sADCx);
        sADCx.ADC_ClockSource = ADC_CLOCK_SOURCE_ADC;
        sADCx.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
        sADCx.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
        sADCx.ADC_ChannelNumber = (uint32_t)channelName;
        sADCx.ADC_Channels = 0;
        sADCx.ADC_LevelControl = ADC_LEVEL_CONTROL_Disable;
        sADCx.ADC_LowLevel = 0;
        sADCx.ADC_HighLevel = 0;
        sADCx.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;
        sADCx.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;
        sADCx.ADC_Prescaler = ADC_CLK_div_None;
        sADCx.ADC_DelayGo = 7;
        ADC1_Init(&sADCx);

        ADC1_Cmd(ENABLE);

        ADC1_Start();
        Rudiron::CLK::delay_micros(10);
        int resultReg = ADC1_GetResult();
        result.channel = (ADCChannelName)(resultReg >> 16);
        result.value = (uint16_t)(resultReg & 0xFFF);

        result.override = (bool)ADC1_GetFlagStatus(ADC1_FLAG_OVERWRITE);
        result.valid = true;

        ADC::lastResult = result;
        ADC1_ClearOverwriteFlag();

        disable();

        return result;
    }

    ADCResult ADC::read_temperature_sensor()
    {
        ADCResult result;
        result.valid = false;

        ADCChannelName channelName = ADCChannelName::ADC_Channel31;

        enable();

        ADC1_Cmd(DISABLE);
        ADC_DeInit();
        ADC_InitTypeDef sADC;
        ADC_StructInit(&sADC);
        sADC.ADC_TempSensor = ADC_TEMP_SENSOR_Enable;
        sADC.ADC_TempSensorAmplifier = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
        sADC.ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Enable;
        ADC_Init(&sADC);

        ADCx_InitTypeDef sADCx;
        ADCx_StructInit(&sADCx);
        sADCx.ADC_ClockSource = ADC_CLOCK_SOURCE_ADC;
        sADCx.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
        sADCx.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
        sADCx.ADC_ChannelNumber = (uint32_t)channelName;
        sADCx.ADC_Channels = 0;
        sADCx.ADC_LevelControl = ADC_LEVEL_CONTROL_Disable;
        sADCx.ADC_LowLevel = 0;
        sADCx.ADC_HighLevel = 0;
        sADCx.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;
        sADCx.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;
        sADCx.ADC_Prescaler = ADC_CLK_div_None;
        sADCx.ADC_DelayGo = 7;
        ADC1_Init(&sADCx);

        ADC1_Cmd(ENABLE);

        ADC1_Start();
        Rudiron::CLK::delay_micros(10);
        int resultReg = ADC1_GetResult();
        result.channel = (ADCChannelName)(resultReg >> 16);
        result.value = (uint16_t)(resultReg & 0xFFF);

        result.override = (bool)ADC1_GetFlagStatus(ADC1_FLAG_OVERWRITE);
        result.valid = true;

        ADC::lastResult = result;
        ADC1_ClearOverwriteFlag();

        disable();

        return result;
    }

    
    ADCResult ADC::read_internal_reference_voltage_source()
    {
        ADCResult result;
        result.valid = false;

        ADCChannelName channelName = ADCChannelName::ADC_Channel30;

        enable();

        ADC1_Cmd(DISABLE);
        ADC_DeInit();
        ADC_InitTypeDef sADC;
        ADC_StructInit(&sADC);
        sADC.ADC_TempSensor = ADC_TEMP_SENSOR_Enable;
        sADC.ADC_TempSensorAmplifier = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
        sADC.ADC_IntVRefConversion = ADC_VREF_CONVERSION_Enable;
        ADC_Init(&sADC);

        ADCx_InitTypeDef sADCx;
        ADCx_StructInit(&sADCx);
        sADCx.ADC_ClockSource = ADC_CLOCK_SOURCE_ADC;
        sADCx.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
        sADCx.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
        sADCx.ADC_ChannelNumber = (uint32_t)channelName;
        sADCx.ADC_Channels = 0;
        sADCx.ADC_LevelControl = ADC_LEVEL_CONTROL_Disable;
        sADCx.ADC_LowLevel = 0;
        sADCx.ADC_HighLevel = 0;
        sADCx.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;
        sADCx.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;
        sADCx.ADC_Prescaler = ADC_CLK_div_None;
        sADCx.ADC_DelayGo = 7;
        ADC1_Init(&sADCx);

        ADC1_Cmd(ENABLE);

        ADC1_Start();
        Rudiron::CLK::delay_micros(10);
        int resultReg = ADC1_GetResult();
        result.channel = (ADCChannelName)(resultReg >> 16);
        result.value = (uint16_t)(resultReg & 0xFFF);

        result.override = (bool)ADC1_GetFlagStatus(ADC1_FLAG_OVERWRITE);
        result.valid = true;

        ADC::lastResult = result;
        ADC1_ClearOverwriteFlag();

        disable();

        return result;
    }
}
