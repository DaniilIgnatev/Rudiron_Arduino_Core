#include "timer.h"
#include "clk.h"

namespace Rudiron
{

    MDR_TIMER_TypeDef *Timer::get_MDR_TIMER() const
    {
        return MDR_TIMER;
    }

    uint16_t Timer::get_ARR() const
    {
        return ARR;
    }

    Timer::Timer(TimerName name)
    {
        this->name = name;

        switch (this->name)
        {
        case Timer_1:
            this->MDR_TIMER = MDR_TIMER1;
            this->RST_CLK_PCLK = RST_CLK_PCLK_TIMER1;
            this->TIMER_IRQn = TIMER1_IRQn;
            this->DMA_Channel = DMA_Channel_TIM1;
            Timer_1_DMA_Interrupt_Handler = &DMA_Interrupt_Handler_1;
            break;
        case Timer_2:
            this->MDR_TIMER = MDR_TIMER2;
            this->RST_CLK_PCLK = RST_CLK_PCLK_TIMER2;
            this->TIMER_IRQn = TIMER2_IRQn;
            this->DMA_Channel = DMA_Channel_TIM2;
            Timer_2_DMA_Interrupt_Handler = &DMA_Interrupt_Handler_2;
            break;
        case Timer_3:
            this->MDR_TIMER = MDR_TIMER3;
            this->RST_CLK_PCLK = RST_CLK_PCLK_TIMER3;
            this->TIMER_IRQn = TIMER3_IRQn;
            this->DMA_Channel = DMA_Channel_TIM3;
            Timer_3_DMA_Interrupt_Handler = &DMA_Interrupt_Handler_3;
            break;
        default:
            return;
        }

        enable_unit();
    }

    Timer::~Timer()
    {
        disable_unit();
    }

    void Timer::enable_unit()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK, ENABLE);
    }

    void Timer::disable_unit()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK, DISABLE);
    }

    void Timer::enable()
    {
        TIMER_Cmd(this->MDR_TIMER, ENABLE);
        NVIC_EnableIRQ(TIMER_IRQn);
    }

    void Timer::disable()
    {
        NVIC_DisableIRQ(TIMER_IRQn);
        TIMER_Cmd(this->MDR_TIMER, DISABLE);
    }

    bool Timer::isHighFrequency()
    {
        return this->frequency > 50000;
    }

    void Timer::setup(uint32_t frequency)
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
        TIMER_BRGInit(this->MDR_TIMER, (isHighFrequency ? TIMER_HCLKdiv1 : CLK::getHCLKdiv() + 3));

        uint32_t timer_herz = Rudiron::CLK::getCPUFrequency();
        if (!isHighFrequency)
        {
            timer_herz = timer_herz / CLK::getCPU_Multiplier() / 8;
        }

        uint32_t fullARR = timer_herz / frequency;
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

    void Timer::PWM_start(PortPinName pinName, uint16_t width, uint16_t width_max)
    {
        PWM_initPin(pinName);
        PWM_activateChannel(pinName, width, width_max, false);
        enable();
    }

    void Timer::PWM_start(PortPinName pinName, PortPinName invertedPinName, uint16_t width, uint16_t width_max)
    {
        PWM_initPin(pinName);
        PWM_initPin(invertedPinName);
        PWM_activateChannel(pinName, width, width_max, true);
        enable();
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

        uint16_t pin = GPIO::get_arduino_gpio(pinName);
        PORT_InitStructure.PORT_Pin = pin;
        PORT_InitStructure.PORT_OE = PORT_OE_OUT;
        PORT_InitStructure.PORT_FUNC = func;
        PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
        PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;

        return PORT_InitStructure;
    }

    int Timer::PWM_activateChannel(PortPinName pinName, uint16_t width, uint16_t width_max, bool withNegative, bool ignoreCompare)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        if (!descriptor.has)
        {
            return -1;
        }

        if (width >= width_max)
        {
            GPIO::configPinOutput(pinName);
            GPIO::writePin(pinName, true);
            return -1;
        }
        if (width == 0)
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
            TIMER_SetChnCompare(this->MDR_TIMER, TIMER_CHANNEL, (double)width * (double)this->ARR / (double)width_max);
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

    uint8_t Timer::getDmaChannel() const
    {
        return DMA_Channel;
    }

    void Timer::DMA_Interrupt_Handler_1()
    {
        Timer &timer = getTimer1();
        if (timer.dma_interrupt_handler)
        {
            timer.dma_interrupt_handler(timer);
        }
    }

    void Timer::DMA_Interrupt_Handler_2()
    {
        Timer &timer = getTimer2();
        if (timer.dma_interrupt_handler)
        {
            timer.dma_interrupt_handler(timer);
        }
    }

    void Timer::DMA_Interrupt_Handler_3()
    {
        Timer &timer = getTimer3();
        if (timer.dma_interrupt_handler)
        {
            timer.dma_interrupt_handler(timer);
        }
    }

    void Timer::PWM_DMA_init(uint32_t channel_number, uint16_t *buffer, uint16_t buffer_length,
                             void (*dma_interrupt_handler)(Timer &timer))
    {
        this->dma_interrupt_handler = dma_interrupt_handler;

        // Run DMA clock
        RST_CLK_PCLKcmd((RST_CLK_PCLK_DMA), ENABLE);

        /* DMA_Channel_TIM1 configuration ---------------------------------*/
        /* Set Primary Control Data */
        DMA_DeInit();
        DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)((uint8_t *)buffer);
        DMA_PriCtrlStr.DMA_DestBaseAddr = (uint32_t)(&(this->MDR_TIMER->CCR1) + channel_number);
        DMA_PriCtrlStr.DMA_SourceIncSize = DMA_SourceIncHalfword;
        DMA_PriCtrlStr.DMA_DestIncSize = DMA_DestIncNo;
        DMA_PriCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_PriCtrlStr.DMA_Mode = DMA_Mode_Basic;
        DMA_PriCtrlStr.DMA_CycleSize = buffer_length;
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

    void Timer::PWM_DMA_setup(PortPinName pinName, uint16_t *buffer, uint16_t buffer_length,
                              void (*dma_interrupt_handler)(Timer &timer))
    {
        PWM_initPin(pinName);
        int channel_number = PWM_activateChannel(pinName, 1, 2, false, true);
        if (channel_number >= 0)
        {
            PWM_DMA_init(channel_number, buffer, buffer_length, dma_interrupt_handler);
            PWM_DMA_repeat();
        }
    }

    void
    Timer::PWM_DMA_setup(PortPinName pinName, PortPinName invertedPinName, uint16_t *buffer, uint16_t buffer_length,
                         void (*dma_interrupt_handler)(Timer &timer))
    {
        PWM_initPin(pinName);
        PWM_initPin(invertedPinName);
        int channel_number = PWM_activateChannel(pinName, 1, 2, true, true);
        if (channel_number >= 0)
        {
            PWM_DMA_init(channel_number, buffer, buffer_length, dma_interrupt_handler);
            PWM_DMA_repeat();
        }
    }

    bool Timer::PWM_DMA_done()
    {
        return !DMA_GetFlagStatus(DMA_Channel, DMA_FLAG_CHNL_ENA);
    }

    void Timer::PWM_DMA_wait_done()
    {
        while ((DMA_GetFlagStatus(DMA_Channel, DMA_FLAG_CHNL_ENA)))
        {
        }
    }

    void Timer::PWM_DMA_repeat()
    {
        PWM_DMA_wait_done();
        DMA_Init(DMA_Channel, &DMA_InitStr);
        enable();
    }

    void Timer::PWM_DMA_update(uint16_t *buffer, uint16_t buffer_length)
    {
        while (DMA_GetFlagStatus(DMA_Channel, DMA_FLAG_CHNL_ENA))
        {
        }

        DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)((uint8_t *)buffer);
        DMA_PriCtrlStr.DMA_CycleSize = buffer_length;

        DMA_Init(DMA_Channel, &DMA_InitStr);
        enable();
    }

    void Timer::PWM_DMA_stop()
    {
        TIMER_DMACmd(this->MDR_TIMER, (TIMER_STATUS_CNT_ARR), DISABLE);
    }

    TimerName Timer::getTimerNameForPin(PortPinName pinName)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        if (!descriptor.has)
        {
            return Timer_None;
        }
        else
        {
            return descriptor.timer_name;
        }
    }

    Timer &Timer::getTimer_by_name(TimerName name)
    {
        switch (name)
        {
        case TimerName::Timer_2:
            return Timer::getTimer2();
        case TimerName::Timer_3:
            return Timer::getTimer3();
        default:
            return Timer::getTimer1();
        }
    }

    bool Timer::hasTimer_for_pinName(PortPinName pinName)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        return descriptor.has;
    }

    Timer &Timer::getTimer_by_pinName(PortPinName name)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(name);
        return getTimer_by_name(descriptor.timer_name);
    }

    Timer &Timer::getTimer1()
    {
        static Timer timer = Timer(TimerName::Timer_1);
        return timer;
    }

    Timer &Timer::getTimer2()
    {
        static Timer timer = Timer(TimerName::Timer_2);
        return timer;
    }

    Timer &Timer::getTimer3()
    {
        static Timer timer = Timer(TimerName::Timer_3);
        return timer;
    }

    void pwm(uint8_t pin, uint32_t frequency, uint16_t width, uint16_t width_max){
        Rudiron::PortPinName pinName = Rudiron::GPIO::get_rudiron_gpio(pin);

        if (Rudiron::Timer::hasTimer_for_pinName(pinName))
        {
            Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_pinName(pinName);
            timer.setup(frequency);
            timer.PWM_start(pinName, width, width_max);
        }
    }
}