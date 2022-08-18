#include "timer.h"
#include "clk.h"

namespace Rudiron
{

    bool Timer::hasTimerForPin(PortPinName pinName)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        return descriptor.has;
    }

    ///Настоятельно рекомендуется проверять методом hasTimerForPin наличие таймера для пина
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

    void Timer::start()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK, ENABLE);
    }

    void Timer::stop()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK, DISABLE);
    }

    Timer::Timer(TimerName name, MDR_TIMER_TypeDef *MDR_TIMER, uint32_t RST_CLK_PCLK)
    {
        this->name = name;
        this->MDR_TIMER = MDR_TIMER;
        this->RST_CLK_PCLK = RST_CLK_PCLK;
    }

    void Timer::PWM_setup(uint16_t frequency)
    {
        TIMER_DeInit(MDR_TIMER);

        RST_CLK_FreqTypeDef RST_CLK_Clocks;
        RST_CLK_GetClocksFreq(&RST_CLK_Clocks);

        uint32_t fullARR = 2000000 / frequency;
        prescaler = fullARR / 0xffff;
        ARR = fullARR - (prescaler * 0xffff);

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
    }

    void Timer::PWM_start(PortPinName pinName, uint16_t ppm)
    {
        PWM_initPin(pinName);
        PWM_activateChannel(pinName, ppm, false);
    }

    void Timer::PWM_start(PortPinName pinName, PortPinName invertedPinName, uint16_t ppm)
    {
        PWM_initPin(pinName);
        PWM_initPin(invertedPinName);
        PWM_activateChannel(pinName, ppm, true);
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

    void Timer::PWM_activateChannel(PortPinName pinName, uint16_t ppm, bool withNegative)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        if (!descriptor.has)
        {
            return;
        }
        
        if (ppm >= 1000){
            GPIO::configPinOutput(pinName);
            GPIO::writePin(pinName, true);
            return;
        }
        if (ppm == 0){
            GPIO::configPinOutput(pinName);
            GPIO::writePin(pinName, false);
            return;
        }

        uint16_t TIMER_CHANNEL = descriptor.channelNum;

        //Режим канала
        TIMER_ChnInitTypeDef sTIM_ChnInit;
        TIMER_ChnStructInit(&sTIM_ChnInit);
        sTIM_ChnInit.TIMER_CH_Mode = TIMER_CH_MODE_PWM;
        sTIM_ChnInit.TIMER_CH_REF_Format = TIMER_CH_REF_Format6;
        sTIM_ChnInit.TIMER_CH_Number = TIMER_CHANNEL;
        TIMER_ChnInit(this->MDR_TIMER, &sTIM_ChnInit);

        //Степень заполнения
        TIMER_SetChnCompare(this->MDR_TIMER, TIMER_CHANNEL, ppm * this->ARR / 1000);

        //Выход канала
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

        /* Enable TIMER1 clock */
        TIMER_BRGInit(this->MDR_TIMER, TIMER_HCLKdiv8 + CLK::getHCLKdiv());
        /* Enable TIMER */
        TIMER_Cmd(this->MDR_TIMER, ENABLE);
    }

    void Timer::PWM_stop(PortPinName pinName)
    {
        TimerChannel_Descriptor descriptor = TimerUtility::getTimerChannel(pinName);
        if (!descriptor.has)
        {
            return;
        }

        uint16_t TIMER_CHANNEL = descriptor.channelNum;

        //Режим канала
        TIMER_ChnInitTypeDef sTIM_ChnInit;
        TIMER_ChnStructInit(&sTIM_ChnInit);
        sTIM_ChnInit.TIMER_CH_Number = TIMER_CHANNEL;
        TIMER_ChnInit(this->MDR_TIMER, &sTIM_ChnInit);

        //Выход канала
        TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;
        TIMER_ChnOutStructInit(&sTIM_ChnOutInit);

        sTIM_ChnOutInit.TIMER_CH_Number = TIMER_CHANNEL;
        TIMER_ChnOutInit(this->MDR_TIMER, &sTIM_ChnOutInit);
    }

    Timer* Timer::getTimerForPinName(PortPinName pinName)
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
        static Timer timer = Timer(TimerName::Timer1, MDR_TIMER1, RST_CLK_PCLK_TIMER1);
        return &timer;
    }

    Timer *Timer::getTimer2()
    {
        static Timer timer = Timer(TimerName::Timer2, MDR_TIMER2, RST_CLK_PCLK_TIMER2);
        return &timer;
    }

    Timer *Timer::getTimer3()
    {
        static Timer timer = Timer(TimerName::Timer3, MDR_TIMER3, RST_CLK_PCLK_TIMER3);
        return &timer;
    }
}