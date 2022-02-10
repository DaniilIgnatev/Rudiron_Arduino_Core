#include "adc.h"

namespace Rudiron {

    void ADC::initPinADC(PortPinName pinName)
    {
        PORT_InitTypeDef PORT_InitStructure;
        uint16_t pin = port.getPinNumber(pinName);

        PORT_InitStructure.PORT_Pin   = pin;
        PORT_InitStructure.PORT_OE    = PORT_OE_IN;
        PORT_InitStructure.PORT_MODE  = PORT_MODE_ANALOG;

        PORT_InitStructure.PORT_FUNC = ::PORT_FUNC_PORT;
        PORT_InitStructure.PORT_GFEN = ::PORT_GFEN_OFF;
        PORT_InitStructure.PORT_PD = ::PORT_PD_DRIVER;
        PORT_InitStructure.PORT_PD_SHM = ::PORT_PD_SHM_OFF;
        PORT_InitStructure.PORT_PULL_DOWN = ::PORT_PULL_DOWN_OFF;
        PORT_InitStructure.PORT_PULL_UP = ::PORT_PULL_UP_OFF;
        PORT_InitStructure.PORT_SPEED = ::PORT_SPEED_FAST;

        port.configPin(pinName, PORT_InitStructure);
    }


    ADC::ADC(ADCName name)
    {
        this->name = name;
    }


    void ADC::configureDefault()
    {
        ADC_InitTypeDef sADC;

        /* ADC Configuration */
        /* Reset all ADC settings */
        ADC_DeInit();
        ADC_StructInit(&sADC);
        ADC_Init (&sADC);

        /* ADC1 EOCIF and AWOIFEN interupts */
        if (this->name == ADC1){
            ADC1_ITConfig((ADCx_IT_END_OF_CONVERSION  | ADCx_IT_OUT_OF_RANGE), DISABLE);
        }
        else{
            ADC2_ITConfig((ADCx_IT_END_OF_CONVERSION  | ADCx_IT_OUT_OF_RANGE), DISABLE);
        }
    }


    bool ADC::configurePin(PortPinName pinName, bool enable)
    {
        PortName portName = port.getPortName(pinName);
        if (portName != PORT_D){
            return false;
        }
        initPinADC(pinName);

        ADCChannelName channelName = (ADCChannelName)(pinName - PORT_PIN_D0);
        if (enable){
            this->channelMask |= (1 << channelName);
        }
        else{
            this->channelMask &= ~(1 << channelName);
        }

        ADCx_InitTypeDef sADCx;
        ADCx_StructInit (&sADCx);
        sADCx.ADC_ClockSource      = ADC_CLOCK_SOURCE_CPU;
        sADCx.ADC_SamplingMode     = ADC_SAMPLING_MODE_CICLIC_CONV;
        sADCx.ADC_ChannelSwitching = ADC_CH_SWITCHING_Enable;
        sADCx.ADC_ChannelNumber    = ADC_CH_TEMP_SENSOR;
        sADCx.ADC_Channels         = this->channelMask;
        sADCx.ADC_LevelControl     = ADC_LEVEL_CONTROL_Disable;
        sADCx.ADC_LowLevel         = 0;
        sADCx.ADC_HighLevel        = 0;
        sADCx.ADC_VRefSource       = ADC_VREF_SOURCE_INTERNAL;
        sADCx.ADC_IntVRefSource    = ADC_INT_VREF_SOURCE_INEXACT;
        sADCx.ADC_Prescaler        = ADC_CLK_div_2048;
        sADCx.ADC_DelayGo          = 7;

        if (this->name == ADC1){
            ADC1_Init (&sADCx);
        }
        else{
            ADC2_Init (&sADCx);
        }


        return true;
    }


    void ADC::start()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);
        ADC1_Cmd (ENABLE);
        ADC1_Start();
    }


    void ADC::stop()
    {
        ADC1_Cmd (DISABLE);
        RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, DISABLE);
    }


    ADCResult ADC::readPin(PortPinName pinName)
    {
        ADCResult result;
        result.valid = false;

        PortName portName = port.getPortName(pinName);
        if (portName != PORT_D){
            return result;
        }

        ADCChannelName channelName = (ADCChannelName)(pinName - PORT_PIN_D0);
        result.channel = channelName;

        uint32_t resultReg = -1;
        uint16_t value = -1;
        uint16_t readChannel = -1;

        while(readChannel != (uint16_t)channelName){
            resultReg = ADC1_GetResult();
            readChannel = resultReg >> 16;
            value = resultReg & 0xFFF;

            if (readChannel == (uint16_t)channelName){
                result.override = (bool)ADC1_GetFlagStatus(ADC1_FLAG_OVERWRITE);
                result.value = value;
                result.valid = true;
            }

            ADC1_ClearOverwriteFlag();
        }

        return result;
    }


}