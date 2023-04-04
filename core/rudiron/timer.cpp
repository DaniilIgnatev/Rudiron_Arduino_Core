#include "timer.h"
#include "clk.h"

namespace Rudiron
{

    bool Timer::hasTimerForPin(PortPinName pinName)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        return descriptor.has;
    }

    /// Настоятельно рекомендуется проверять методом hasTimerForPin наличие таймера для пина
    TimerName Timer::getTimerNameForPin(PortPinName pinName)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        if (!descriptor.has)
        {
            return None;
        }
        else
        {
            return descriptor.timer;
        }
    }

    Timer::Timer(TimerName name)
    {
        this->name = name;

        switch (this->name)
        {
        case Timer1:
            this->MDR_TIMER = MDR_TIMER1;
            this->RST_CLK_PCLK = RST_CLK_PCLK_TIMER1;
            this->TIMER_IRQn = TIMER1_IRQn;
            this->DMA_Channel = DMA_Channel_TIM1;
            break;
        case Timer2:
            this->MDR_TIMER = MDR_TIMER2;
            this->RST_CLK_PCLK = RST_CLK_PCLK_TIMER2;
            this->TIMER_IRQn = TIMER2_IRQn;
            this->DMA_Channel = DMA_Channel_TIM2;
            break;
        case Timer3:
            this->MDR_TIMER = MDR_TIMER3;
            this->RST_CLK_PCLK = RST_CLK_PCLK_TIMER3;
            this->TIMER_IRQn = TIMER3_IRQn;
            this->DMA_Channel = DMA_Channel_TIM3;
            break;
        default:
            return;
        }

        /* Enable TIMER1 clock */
        RST_CLK_PCLKcmd(RST_CLK_PCLK, ENABLE);
    }

    bool Timer::isHighFrequency()
    {
        return this->frequency > 50000;
    }

    void Timer::PWM_setup(uint32_t frequency)
    {
        if (this->frequency == frequency || frequency == 0)
        {
            return;
        }
        else
        {
            this->frequency = frequency;
        }

        TIMER_DeInit(MDR_TIMER);

        RST_CLK_FreqTypeDef RST_CLK_Clocks;
        RST_CLK_GetClocksFreq(&RST_CLK_Clocks);

        bool isHighFrequency = this->isHighFrequency();
        TIMER_BRGInit(this->MDR_TIMER, (isHighFrequency ? TIMER_HCLKdiv1 : TIMER_HCLKdiv16));

        auto timer_herz = Rudiron::CLK::getCPUFrequency();
        if (!isHighFrequency){
            timer_herz /= 16;
        }

        // uint32_t fullARR = (isHighFrequency() ? 16000000 : 1000000) / frequency;
        uint32_t fullARR = timer_herz / frequency;

        // fullARR /= CLK::getCPU_Multiplier();

        prescaler = fullARR / 0xffff;
        ARR = fullARR - (prescaler * 0xffff) - 1;

        /* Initializes the TIMERx Counter ------------------------------------*/
        TIMER_CntInitTypeDef sTIM_CntInit;
        sTIM_CntInit.TIMER_Prescaler = prescaler;
        sTIM_CntInit.TIMER_Period = ARR;
        sTIM_CntInit.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;
        sTIM_CntInit.TIMER_CounterDirection = TIMER_CntDir_Up;
        sTIM_CntInit.TIMER_EventSource = TIMER_EvSrc_None;
        sTIM_CntInit.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
        sTIM_CntInit.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately;
        sTIM_CntInit.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK;
        sTIM_CntInit.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None;
        sTIM_CntInit.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted;
        sTIM_CntInit.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted;
        TIMER_CntInit(MDR_TIMER, &sTIM_CntInit);

        TIMER_ITConfig(this->MDR_TIMER, TIMER_STATUS_CNT_ARR, ENABLE);
        MDR_TIMER->STATUS = 0;
    }

    void Timer::PWM_start(PortPinName pinName, uint16_t ppm)
    {
        PWM_initPin(pinName);
        PWM_activateChannel(pinName, ppm, false);
        enable();
    }

    void Timer::PWM_start(PortPinName pinName, PortPinName invertedPinName, uint16_t ppm)
    {
        PWM_initPin(pinName);
        PWM_initPin(invertedPinName);
        PWM_activateChannel(pinName, ppm, true);
        disable();
    }

    void Timer::PWM_initPin(PortPinName pinName)
    {
        PORT_InitTypeDef PWMInit_MAIN = PWM_getInitTypeDef(pinName, PORT_FUNC_MAIN);
        PORT_InitTypeDef PWMInit_ALTER = PWM_getInitTypeDef(pinName, PORT_FUNC_ALTER);
        PORT_InitTypeDef PWMInit_OVERRID = PWM_getInitTypeDef(pinName, PORT_FUNC_OVERRID);

        PORT_InitTypeDef initTypeDef = TimerUtility::getChannelInit(pinName, PWMInit_MAIN, PWMInit_ALTER,
                                                                    PWMInit_OVERRID);

        GPIO::configPin(pinName, initTypeDef);
    }

    PORT_InitTypeDef Timer::PWM_getInitTypeDef(PortPinName pinName, PORT_FUNC_TypeDef func)
    {
        PORT_InitTypeDef PORT_InitStructure;
        PORT_StructInit(&PORT_InitStructure);

        uint16_t pin = GPIO::getPinNumber(pinName);
        PORT_InitStructure.PORT_Pin = pin;
        PORT_InitStructure.PORT_OE = PORT_OE_OUT;
        PORT_InitStructure.PORT_FUNC = func;
        PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
        PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;

        return PORT_InitStructure;
    }

    int Timer::PWM_activateChannel(PortPinName pinName, uint16_t ppm, bool withNegative, bool ignoreCompare)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        if (!descriptor.has)
        {
            return -1;
        }

        if (ppm >= 1000)
        {
            GPIO::configPinOutput(pinName);
            GPIO::writePin(pinName, true);
            return -1;
        }
        if (ppm == 0)
        {
            GPIO::configPinOutput(pinName);
            GPIO::writePin(pinName, false);
            return -1;
        }

        uint32_t TIMER_CHANNEL = descriptor.channelNum;

        // Режим канала
        TIMER_ChnInitTypeDef sTIM_ChnInit;
        TIMER_ChnStructInit(&sTIM_ChnInit);
        sTIM_ChnInit.TIMER_CH_Mode = TIMER_CH_MODE_PWM;
        sTIM_ChnInit.TIMER_CH_REF_Format = TIMER_CH_REF_Format6;
        sTIM_ChnInit.TIMER_CH_Number = TIMER_CHANNEL;
        TIMER_ChnInit(this->MDR_TIMER, &sTIM_ChnInit);

        // Степень заполнения
        if (!ignoreCompare)
        {
            TIMER_SetChnCompare(this->MDR_TIMER, TIMER_CHANNEL, ppm * this->ARR / 1000);
        }

        // Выход канала
        TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;
        TIMER_ChnOutStructInit(&sTIM_ChnOutInit);

        sTIM_ChnOutInit.TIMER_CH_DirOut_Polarity = TIMER_CHOPolarity_NonInverted;
        sTIM_ChnOutInit.TIMER_CH_DirOut_Source = TIMER_CH_OutSrc_REF;
        sTIM_ChnOutInit.TIMER_CH_DirOut_Mode = TIMER_CH_OutMode_Output;

        if (withNegative)
        {
            sTIM_ChnOutInit.TIMER_CH_DirOut_Polarity = TIMER_CHOPolarity_NonInverted;
        }
        else
        {
            sTIM_ChnOutInit.TIMER_CH_NegOut_Polarity = TIMER_CHOPolarity_Inverted;
        }

        sTIM_ChnOutInit.TIMER_CH_NegOut_Source = TIMER_CH_OutSrc_REF;
        sTIM_ChnOutInit.TIMER_CH_NegOut_Mode = TIMER_CH_OutMode_Output;

        sTIM_ChnOutInit.TIMER_CH_Number = TIMER_CHANNEL;
        TIMER_ChnOutInit(this->MDR_TIMER, &sTIM_ChnOutInit);

        return TIMER_CHANNEL;
    }

    void Timer::PWM_stop(PortPinName pinName)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        if (!descriptor.has)
        {
            return;
        }

        uint16_t TIMER_CHANNEL = descriptor.channelNum;

        // Режим канала
        TIMER_ChnInitTypeDef sTIM_ChnInit;
        TIMER_ChnStructInit(&sTIM_ChnInit);
        sTIM_ChnInit.TIMER_CH_Number = TIMER_CHANNEL;
        TIMER_ChnInit(this->MDR_TIMER, &sTIM_ChnInit);

        // Выход канала
        TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;
        TIMER_ChnOutStructInit(&sTIM_ChnOutInit);

        sTIM_ChnOutInit.TIMER_CH_Number = TIMER_CHANNEL;
        TIMER_ChnOutInit(this->MDR_TIMER, &sTIM_ChnOutInit);
    }

    void Timer::PWM_DMA_init(uint32_t channel_number, uint16_t *buffer, uint16_t buffer_length)
    {
        this->DMA_TIMER_CHANNEL = channel_number;
        this->BUF_DMA = (uint8_t *)buffer;
        this->BUF_DMA_LENGTH = buffer_length;

        // Run DMA clock
        RST_CLK_PCLKcmd((RST_CLK_PCLK_DMA), ENABLE);

        /* DMA_Channel_TIM1 configuration ---------------------------------*/
        /* Set Primary Control Data */
        DMA_DeInit();
        DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)BUF_DMA;
        DMA_PriCtrlStr.DMA_DestBaseAddr = (uint32_t)(&(this->MDR_TIMER->CCR1) + channel_number);
        DMA_PriCtrlStr.DMA_SourceIncSize = DMA_SourceIncHalfword;
        DMA_PriCtrlStr.DMA_DestIncSize = DMA_DestIncNo;
        DMA_PriCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_PriCtrlStr.DMA_Mode = DMA_Mode_Basic;
        DMA_PriCtrlStr.DMA_CycleSize = BUF_DMA_LENGTH;
        DMA_PriCtrlStr.DMA_NumContinuous = DMA_Transfers_1;
        DMA_PriCtrlStr.DMA_SourceProtCtrl = DMA_SourcePrivileged;
        DMA_PriCtrlStr.DMA_DestProtCtrl = DMA_DestPrivileged;

        /* Set Channel Structure */
        DMA_StructInit(&DMA_InitStr);
        DMA_InitStr.DMA_PriCtrlData = &DMA_PriCtrlStr;
        DMA_InitStr.DMA_Priority = DMA_Priority_High;
        DMA_InitStr.DMA_UseBurst = DMA_BurstClear;
        DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;

        /* Enable TIMER DMA request */
        TIMER_DMACmd(this->MDR_TIMER, (TIMER_STATUS_CNT_ARR), ENABLE);
    }

    void Timer::PWM_DMA_setup(PortPinName pinName, uint16_t *buffer, uint16_t buffer_length)
    {
        PWM_initPin(pinName);
        int channel_number = PWM_activateChannel(pinName, 500, false, true);
        if (channel_number >= 0)
        {
            PWM_DMA_init(channel_number, buffer, buffer_length);
        }
    }

    void Timer::PWM_DMA_setup(PortPinName pinName, PortPinName invertedPinName, uint16_t *buffer, uint16_t buffer_length)
    {
        PWM_initPin(pinName);
        PWM_initPin(invertedPinName);
        int channel_number = PWM_activateChannel(pinName, 500, true, true);
        if (channel_number >= 0)
        {
            PWM_DMA_init(channel_number, buffer, buffer_length);
        }
    }

    void Timer::PWM_DMA_setBuffer(uint16_t *buffer, uint16_t buffer_length)
    {
        this->BUF_DMA = (uint8_t *)buffer;
        this->BUF_DMA_LENGTH = buffer_length;

        DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)BUF_DMA;
        DMA_PriCtrlStr.DMA_CycleSize = BUF_DMA_LENGTH;
    }

    void Timer::PWM_DMA_start_single()
    {
        while ((DMA_GetFlagStatus(DMA_Channel, DMA_FLAG_CHNL_ENA)))
        {
        }

        DMA_Init(DMA_Channel, &DMA_InitStr);
        enable();
    }

    void Timer::PWM_DMA_start_single(uint16_t *buffer, uint16_t buffer_length)
    {
        while (DMA_GetFlagStatus(DMA_Channel, DMA_FLAG_CHNL_ENA))
        {
        }

        PWM_DMA_setBuffer(buffer, buffer_length);
        DMA_Init(DMA_Channel, &DMA_InitStr);
        enable();
    }

    void Timer::PWM_DMA_stop()
    {
        TIMER_DMACmd(this->MDR_TIMER, (TIMER_STATUS_CNT_ARR), DISABLE);
    }

    Timer *Timer::getTimerForPinName(PortPinName pinName)
    {
        auto descriptor = TimerUtility::getTimerChannel(pinName);

        switch (descriptor.timer)
        {
        case TimerName::Timer1:
            return Timer::getTimer1();
        case TimerName::Timer2:
            return Timer::getTimer2();
        case TimerName::Timer3:
            return Timer::getTimer3();
        default:
            return nullptr;
        }
    }

    Timer *Timer::getTimer1()
    {
        static Timer timer = Timer(TimerName::Timer1);
        return &timer;
    }

    Timer *Timer::getTimer2()
    {
        static Timer timer = Timer(TimerName::Timer2);
        return &timer;
    }

    Timer *Timer::getTimer3()
    {
        static Timer timer = Timer(TimerName::Timer3);
        return &timer;
    }
}