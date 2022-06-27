#include "clk.h"

namespace Rudiron
{
    CLK_Speed _CLK_Speed = CLK_Speed::low;

    uint32_t _CPU_Multiplier = DEFAULT_RST_CLK_CPU_PLLmul;

    uint32_t _HCLKdiv = (uint32_t)0x00000000;

    void CLK::setCPUSpeed(CLK_Speed newValue)
    {
        _CLK_Speed = newValue;

        switch (_CLK_Speed)
        {
        case CLK_Speed::medium:
            _CPU_Multiplier = ((DEFAULT_RST_CLK_CPU_PLLmul + 1) * 2) - 1;
            _HCLKdiv = ((uint32_t)0x00000001);
            break;
        case CLK_Speed::high:
            _CPU_Multiplier = ((DEFAULT_RST_CLK_CPU_PLLmul + 1) * 4) - 1;
            _HCLKdiv = ((uint32_t)0x00000002);
            break;
        default:
            _CPU_Multiplier = DEFAULT_RST_CLK_CPU_PLLmul;
            _HCLKdiv = ((uint32_t)0x00000000);
            break;
        }

        updateHighSpeedConfig();
    }

    void CLK::updateHighSpeedConfig()
    {
        RST_CLK_DeInit();
#ifndef HCLK_DISABLE
        RST_CLK_HSEconfig(RST_CLK_HSE_ON);
        while (RST_CLK_HSEstatus() != SUCCESS)
        {
        };
        CLK::runHSE(getCPU_Multiplier());
#else
        RST_CLK_HSEconfig(RST_CLK_HSE_OFF);
        CLK::runHSI(getCPU_Multiplier());
#endif
    }

    ///Выбор внешнего источника тактирования и коэффициента умножения частоты
    void CLK::runHSE(uint32_t RST_CLK_CPU_PLLmul)
    {
        RST_CLK_HSEconfig(RST_CLK_HSE_ON);
        if (RST_CLK_HSEstatus() == SUCCESS)
        {
            RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul);
            RST_CLK_CPU_PLLcmd(ENABLE);
            if (RST_CLK_CPU_PLLstatus() == SUCCESS)
            {
                updateEEPROMLatency(true, RST_CLK_CPU_PLLmul);
                RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
                RST_CLK_CPU_PLLuse(ENABLE);
                RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
            }
        }
        init_delay();
    }

    ///Выбор внутреннего источника тактирования и коэффициента умножения частоты
    void CLK::runHSI(uint32_t RST_CLK_CPU_PLLmul)
    {
        RST_CLK_HSIcmd(ENABLE);
        RST_CLK_CPU_PLLcmd(ENABLE);
        RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSIdiv1, RST_CLK_CPU_PLLmul);

        if (RST_CLK_HSIstatus() == SUCCESS)
        {
            updateEEPROMLatency(false, RST_CLK_CPU_PLLmul);
            RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
            RST_CLK_CPU_PLLuse(ENABLE);
            RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
        }
        init_delay();
    }

    void CLK::updateEEPROMLatency(bool external, uint32_t RST_CLK_CPU_PLLmul)
    {
        RST_CLK_FreqTypeDef clock_descriptor = getCLKDescriptor();
        uint32_t def_freq = external ? clock_descriptor.RTCHSE_Frequency : clock_descriptor.RTCHSI_Frequency;

        uint32_t result_freq = def_freq * (RST_CLK_CPU_PLLmul + 1);
        uint8_t latency_tics = result_freq / 25000000;

        switch (latency_tics)
        {
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

    void CLK::init_delay()
    {
        RST_CLK_FreqTypeDef RST_CLK_Clocks;
        RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
        uint32_t SystemCoreClock = RST_CLK_Clocks.CPU_CLK_Frequency;

        // Set reload register to generate IRQ every microsecond
        SysTick->LOAD = (uint32_t)((SystemCoreClock / 1000000 * MICROS_STEP) - 1);

        // Set priority for SysTick IRQ
        NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

        // Set the SysTick counter value
        SysTick->VAL = 0UL;

        // Configure SysTick source and enable counter
        SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    }

    void CLK::delay_millis(uint32_t ms)
    {
        const uint64_t target_micros = _micros + ms * 1000;
        while (_micros < target_micros)
        {
        }
    }

    void CLK::delay_micros(uint32_t us)
    {
        const uint64_t target_micros = _micros + us;
        while (_micros < target_micros)
        {
        }
    }
}
