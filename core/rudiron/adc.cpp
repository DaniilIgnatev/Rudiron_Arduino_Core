#include "adc.h"
#include "gpio.h"

namespace Rudiron
{
    void ADC::begin()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);

        /* Reset all ADC settings */
        ADC_DeInit();
        ADC_InitTypeDef sADC;
        ADC_StructInit(&sADC);
        ADC_Init(&sADC);

        ADCx_InitTypeDef sADCx;
        ADCx_StructInit(&sADCx);
        sADCx.ADC_ClockSource = ADC_CLOCK_SOURCE_CPU;
        sADCx.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;
        sADCx.ADC_ChannelSwitching = ADC_CH_SWITCHING_Enable;
        sADCx.ADC_ChannelNumber = 0;
        sADCx.ADC_Channels = 0;
        sADCx.ADC_LevelControl = ADC_LEVEL_CONTROL_Disable;
        sADCx.ADC_LowLevel = 0;
        sADCx.ADC_HighLevel = 0;
        sADCx.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;
        sADCx.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;
        sADCx.ADC_Prescaler = ADC_CLK_div_2048; // уменьшить для увеличения скорости выборки
        sADCx.ADC_DelayGo = 7;

        ADC1_Init(&sADCx);

        /* EOCIF and AWOIFEN interupts */
        ADC1_ITConfig((ADCx_IT_OUT_OF_RANGE), DISABLE);
        ADC1_ITConfig((ADCx_IT_END_OF_CONVERSION), ENABLE);
        NVIC_EnableIRQ(ADC_IRQn);
    }

    void ADC::end()
    {
        ADC1_Cmd(DISABLE);
        RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, DISABLE);
    }

    uint32_t ADC::channelMask = 0;

    bool ADC::configurePin(PortPinName pinName, bool enable)
    {
        uint32_t old_channelMask = channelMask;

        PortName portName = GPIO::getPortName(pinName);
        if (portName != PORT_D)
        {
            return false;
        }

        ADC::initPinADC(pinName);

        ADCChannelName channelName = (ADCChannelName)(pinName - PORT_PIN_D0);
        if (enable)
        {
            ADC::channelMask |= (1 << channelName);
        }
        else
        {
            ADC::channelMask &= ~(1 << channelName);
        }

        if (old_channelMask != channelMask)
        {
            ADC1_Cmd(DISABLE);
            ADC1_SetChannels(ADC::channelMask);
            ADC1_Cmd(ENABLE);
            ADC1_Start();
        }

        return true;
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

    ADCResult ADC::readPin(PortPinName pinName)
    {
        ADCResult result;
        result.valid = false;

        PortName portName = GPIO::getPortName(pinName);
        if (portName != PORT_D)
        {
            return result;
        }

        ADCChannelName channelName;

        switch (pinName)
        {
        case PORT_PIN_D2: // A3
            channelName = ADC_Channel2;
            break;
        case PORT_PIN_D3: // A2
            channelName = ADC_Channel3;
            break;
        case PORT_PIN_D4: // A4
            channelName = ADC_Channel4;
            break;
        case PORT_PIN_D5: // A1
            channelName = ADC_Channel5;
            break;
        case PORT_PIN_D6: // A0
            channelName = ADC_Channel6;
            break;
        case PORT_PIN_D7: // A5
            channelName = ADC_Channel7;
            break;
        default:
            return result;
        }

        result.channel = channelName;

        result.override = (_adc_buffer + (uint8_t)channelName)->override;
        result.value = (_adc_buffer + (uint8_t)channelName)->value;
        result.valid = (_adc_buffer + (uint8_t)channelName)->valid;
        (_adc_buffer + (uint8_t)channelName)->valid = false;

        return result;
    }
}
