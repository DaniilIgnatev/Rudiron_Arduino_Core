#include "clk.h"

namespace Rudiron {

    void CLK::updateLatency(bool external, uint32_t RST_CLK_CPU_PLLmul) {
        auto clocks = getCLKDescriptor();
        uint32_t def_freq = clocks.RTCHSI_Frequency;

        if (external) {
            def_freq = clocks.RTCHSE_Frequency;
        }

        uint32_t result_freq = def_freq * (RST_CLK_CPU_PLLmul + 1);
        uint8_t latency_tics = result_freq / 25000000;

        switch (latency_tics) {
            case 0:
                EEPROM_SetLatency(EEPROM_Latency_0);
                break;
            case 1:
                EEPROM_SetLatency(EEPROM_Latency_1);
                break;
            case 2:
                EEPROM_SetLatency(EEPROM_Latency_2);
                break;
            case 3:
                EEPROM_SetLatency(EEPROM_Latency_3);
                break;
            case 4:
                EEPROM_SetLatency(EEPROM_Latency_4);
                break;
            case 5:
                EEPROM_SetLatency(EEPROM_Latency_5);
                break;
            case 6:
                EEPROM_SetLatency(EEPROM_Latency_6);
                break;
            default:
                EEPROM_SetLatency(EEPROM_Latency_7);
                break;
        }
    }


    void CLK::Delay_Init() {
        RST_CLK_FreqTypeDef RST_CLK_Clocks;
        RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
        uint32_t SystemCoreClock = RST_CLK_Clocks.CPU_CLK_Frequency;

        // Set reload register to generate IRQ every microsecond
        SysTick->LOAD = (uint32_t) ((SystemCoreClock / 1000000) - 1);

        // Set priority for SysTick IRQ
        NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

        // Set the SysTick counter value
        SysTick->VAL = 0UL;

        // Configure SysTick source and enable counter
        SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    }


    void CLK::delay_millis(uint32_t ms) {
        __IO uint32_t target_counter = _millis + ms;

        while (_millis != target_counter) {}
    }


    void CLK::delay_micros(uint32_t ms) {
        if (ms >= 1000) {
            uint32_t target_millis = ms / 1000;
            delay_millis(target_millis);
            ms = ms % 1000;
        }
        __IO uint32_t delay_counter = ms;

        while (delay_counter) {
            if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
                delay_counter--;
            }
        }
    }


    ///Выбор внешнего источника тактирования и коэффициента умножения частоты, 8Мгц
    void CLK::runHSE(uint32_t RST_CLK_CPU_PLLmul) {
        RST_CLK_HSEconfig(RST_CLK_HSE_ON);
        if (RST_CLK_HSEstatus() == SUCCESS)                     /* Good HSE clock */
        {
            /* Select HSE clock as CPU_PLL input clock source */
            /* Set PLL multiplier to 7                        */
            RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul);
            /* Enable CPU_PLL */
            RST_CLK_CPU_PLLcmd(ENABLE);
            if (RST_CLK_CPU_PLLstatus() == SUCCESS)                     /* Good CPU PLL */
            {
                updateLatency(true, RST_CLK_CPU_PLLmul);
                /* Set CPU_C3_prescaler to 2 */
                RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
                /* Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock */
                RST_CLK_CPU_PLLuse(ENABLE);
                /* Select CPU_C3 clock on the CPU clock MUX */
                RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

                /* LED1 blinking with 7*HSE/2 clock as input clock source */
            }
        }
        Delay_Init();
    }


    ///Выбор внутреннего источника тактирования и коэффициента умножения частоты, 8Мгц
    void CLK::runHSI(uint32_t RST_CLK_CPU_PLLmul) {
        /* Enable HSI clock source */
        RST_CLK_HSIcmd(ENABLE);
        /* Disable CPU_PLL */
        RST_CLK_CPU_PLLcmd(DISABLE);
        /* Select HSI/2 clock as CPU_PLL input clock source */
        RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSIdiv1, RST_CLK_CPU_PLLmul);
        if (RST_CLK_HSIstatus() == SUCCESS)/* Good HSI clock */
        {
            updateLatency(false, RST_CLK_CPU_PLLmul);
            /* Set CPU_C3_prescaler to 1 */
            RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
            /* Switch CPU_C2_SEL to CPU_C1 clock instead of CPU_PLL output */
            RST_CLK_CPU_PLLuse(DISABLE);
            /* Select CPU_C3 clock on the CPU             clock MUX */
            RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
            /* LED1 blinking with HSI/2 clock as input clock source */
        }
        Delay_Init();
    }


    void CLK::IRQ_INIT() {
        SCB->AIRCR = 0x05FA0000 | ((uint32_t) 0x500);
        SCB->VTOR = 0x08000000;
        __enable_irq();
    }


    void CLK::Initialize() {
        RST_CLK_DeInit();
        RST_CLK_HSEconfig(RST_CLK_LSE_OFF);

#ifndef HCLK_DISABLE
        RST_CLK_HSEconfig(RST_CLK_HSE_ON);
        while (RST_CLK_HSEstatus() != SUCCESS);
        CLK::runHSE(RST_CLK_CPU_PLLmul2);
#else
        Clk::runHSI(RST_CLK_CPU_PLLmul1);
#endif
        RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK), ENABLE);
        RST_CLK_PCLKcmd((RST_CLK_PCLK_SSP1), ENABLE);

        IRQ_INIT();
    }
}