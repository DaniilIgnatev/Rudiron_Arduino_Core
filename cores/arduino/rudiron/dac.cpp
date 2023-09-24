/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron_MDR32F9Qx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron_MDR32F9Qx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron_MDR32F9Qx. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
*/

#include "dac.h"

#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_dac.h"
#include "MDR32F9Qx_dma.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_dma.h"

#include "clk.h"

namespace Rudiron
{
    DAC::DAC(DAC_Name dac_name)
    {
        this->dac_name = dac_name;

        enable_unit();
        DAC_DMA_Interrupt_Handler = &DMA_Interrupt_Handler;
    }

    DAC::~DAC()
    {
        disable_unit();
    }

    void DAC::enable_unit()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK_DAC, ENABLE);
    }

    void DAC::disable_unit()
    {
        DAC_DeInit();
        RST_CLK_PCLKcmd(RST_CLK_PCLK_DAC, DISABLE);
    }

    void DAC::enable()
    {
        switch (dac_name)
        {
        case DAC_1:
            DAC1_Cmd(ENABLE);
            break;
        case DAC_2:
            DAC2_Cmd(ENABLE);
            break;
        default:
            // ошибка
            break;
        }
    }

    void DAC::disable()
    {
        switch (dac_name)
        {
        case DAC_1:
            DAC1_Cmd(DISABLE);
            break;
        case DAC_2:
            DAC2_Cmd(DISABLE);
            break;
        default:
            // ошибка
            break;
        }
    }

    uint32_t DAC::getValue()
    {
        switch (dac_name)
        {
        case DAC_1:
            return DAC1_GetData();
        case DAC_2:
            return DAC2_GetData();
        default:
            // ошибка
            return 0;
        }
    }

    void DAC::setValue(uint32_t value)
    {
        switch (dac_name)
        {
        case DAC_1:
            DAC1_SetData(value);
            break;
        case DAC_2:
            DAC2_SetData(value);
            break;
        default:
            // ошибка
            break;
        }
    }

    void DAC::initPinDAC_Ref(PortPinName pinName)
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

    uint32_t DAC::DAC1_Ref = DAC1_AVCC;

    uint32_t DAC::DAC2_Ref = DAC2_AVCC;

    void DAC::set_reference(bool external)
    {
        switch (this->dac_name)
        {
        case DAC_2:
            initPinDAC_Ref(PORT_PIN_E1);
            DAC::DAC2_Ref = external ? DAC2_REF : DAC2_AVCC;
            break;
        default:
            break;
        }

        DAC_Init(DAC_SYNC_MODE_Independent, DAC1_Ref, DAC2_Ref);
    }

    void DAC::initPinDAC_Data(PortPinName pinName)
    {
        PORT_InitTypeDef PORT_InitStructure;
        uint16_t pin = GPIO::get_arduino_gpio(pinName);

        PORT_InitStructure.PORT_Pin = pin;
        PORT_InitStructure.PORT_OE = PORT_OE_OUT;
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

    bool DAC::activate_pin(PortPinName pinName)
    {
        DAC_ChannelName channelName = DAC_Utility::getDAC_ChannelName(pinName);
        DAC_Name dac_name = DAC_Utility::getDAC_Name(channelName);

        if (dac_name != this->dac_name)
        {
            // ошибка
            return false;
        }

        initPinDAC_Data(pinName);

        return true;
    }

    void DAC::write_pin_single(PortPinName pinName, uint32_t value)
    {
        if (!activate_pin(pinName))
        {
            return; // ошибка
        }

        setValue(value);
        enable();
    }

    void DAC::DMA_Interrupt_Handler()
    {
        DAC &dac1 = DAC::getDAC1();
        if (dac1.dma_interrupt_handler)
        {
            dac1.dma_interrupt_handler(dac1);
        }

        DAC &dac2 = DAC::getDAC2();
        if (dac2.dma_interrupt_handler)
        {
            dac2.dma_interrupt_handler(dac2);
        }
    }

    const uint32_t DAC::get_MDR_DAC_Data_Address()
    {
        switch (dac_name)
        {
        case DAC_1:
            return (uint32_t)(&(MDR_DAC->DAC1_DATA));
        default:
            return (uint32_t)(&(MDR_DAC->DAC2_DATA));
        }
    }

    void DAC::DMA_setup(TimerName timer_name,
                        uint32_t frequency,
                        PortPinName pinName,
                        uint16_t *buffer_primary,
                        uint16_t buffer_primary_length,
                        void (*dma_interrupt_handler)(DAC &dac),
                        DMA_Number_Continuous_Transfers primary_transfers,
                        bool alternate_buffer,
                        uint16_t *buffer_alternate,
                        uint16_t buffer_alternate_length,
                        DMA_Number_Continuous_Transfers alternate_transfers)
    {

        if (timer_name == Timer_None)
        {
            return; // ошибка
        }

        this->timer_name = timer_name;
        Timer &timer = Timer::getTimer_by_name(timer_name);
        dma_channel = timer.getDmaChannel();

        if (!activate_pin(pinName))
        {
            return; // ошибка
        }

        this->dma_interrupt_handler = dma_interrupt_handler;
        //        __NVIC_ClearPendingIRQ(DMA_IRQn);

        // Run DMA clock
        RST_CLK_PCLKcmd((RST_CLK_PCLK_DMA), ENABLE);

        /* DMA Configuration */
        /* Reset all settings */
        DMA_DeInit();
        DMA_StructInit(&DMA_InitStr);
        /* Set Primary Control Data */
        DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)buffer_primary;
        DMA_PriCtrlStr.DMA_DestBaseAddr = get_MDR_DAC_Data_Address();
        DMA_PriCtrlStr.DMA_SourceIncSize = DMA_SourceIncHalfword;
        DMA_PriCtrlStr.DMA_DestIncSize = DMA_DestIncNo;
        DMA_PriCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_PriCtrlStr.DMA_Mode = alternate_buffer ? DMA_Mode_PingPong : DMA_Mode_Basic;
        DMA_PriCtrlStr.DMA_CycleSize = buffer_primary_length;
        DMA_PriCtrlStr.DMA_NumContinuous = DMA_Number_Continuous_Transfers_toNumber(primary_transfers);
        DMA_PriCtrlStr.DMA_SourceProtCtrl = DMA_SourcePrivileged;
        DMA_PriCtrlStr.DMA_DestProtCtrl = DMA_DestPrivileged;
        /* Set Alternate Control Data */
        DMA_AltCtrlStr.DMA_SourceBaseAddr = (uint32_t)buffer_alternate;
        DMA_AltCtrlStr.DMA_DestBaseAddr = get_MDR_DAC_Data_Address();
        DMA_AltCtrlStr.DMA_SourceIncSize = DMA_SourceIncHalfword;
        DMA_AltCtrlStr.DMA_DestIncSize = DMA_DestIncNo;
        DMA_AltCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_AltCtrlStr.DMA_Mode = alternate_buffer ? DMA_Mode_PingPong : DMA_Mode_Basic;
        DMA_AltCtrlStr.DMA_CycleSize = buffer_alternate_length;
        DMA_AltCtrlStr.DMA_NumContinuous = DMA_Number_Continuous_Transfers_toNumber(alternate_transfers);
        DMA_AltCtrlStr.DMA_SourceProtCtrl = DMA_SourcePrivileged;
        DMA_AltCtrlStr.DMA_DestProtCtrl = DMA_DestPrivileged;
        /* Set Channel Structure */
        DMA_InitStr.DMA_PriCtrlData = &DMA_PriCtrlStr;
        DMA_InitStr.DMA_AltCtrlData = &DMA_AltCtrlStr;
        DMA_InitStr.DMA_Priority = DMA_Priority_Default;
        DMA_InitStr.DMA_UseBurst = DMA_BurstClear;
        DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;
        DMA_Init(dma_channel, &DMA_InitStr);
        DMA_Cmd(dma_channel, ENABLE);

        enable();

        timer.setup(frequency);
        MDR_TIMER_TypeDef *mdr_timer = timer.get_MDR_TIMER();
        TIMER_DMACmd(mdr_timer, (TIMER_STATUS_CNT_ARR), ENABLE);
        timer.enable();

        /* Enable DMA IRQ */
        //        NVIC_EnableIRQ(DMA_IRQn);
    }

    void DAC::DMA_repeat()
    {
        DMA_Init(dma_channel, &DMA_InitStr);
        enable();
    }

    void DAC::DMA_update(uint16_t *buffer, uint16_t buffer_length, bool primary_control_data)
    {
        if (primary_control_data)
        {
            DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)buffer;
            DMA_PriCtrlStr.DMA_CycleSize = buffer_length;
        }
        else
        {
            DMA_AltCtrlStr.DMA_SourceBaseAddr = (uint32_t)buffer;
            DMA_AltCtrlStr.DMA_CycleSize = buffer_length;
        }

        DMA_repeat();
    }

    bool DAC::DMA_done()
    {
        return !(MDR_DMA->CHNL_ENABLE_SET & (1 << dma_channel));
    }

    void DAC::DMA_wait_done()
    {
        while (!DMA_done())
        {
        }
        return;
    }

    bool DAC::DMA_is_alternate_active()
    {
        return (MDR_DMA->CHNL_PRI_ALT_SET & (1 << dma_channel)) == (0 << dma_channel);
    }

    void DAC::DMA_stop()
    {
        Timer &timer = Timer::getTimer_by_name(timer_name);
        MDR_TIMER_TypeDef *mdr_timer = timer.get_MDR_TIMER();
        TIMER_DMACmd(mdr_timer, (TIMER_STATUS_CNT_ARR), DISABLE);
    }

    DAC &DAC::getDAC1()
    {
        static DAC dac = DAC(DAC_Name::DAC_1);
        return dac;
    }

    DAC &DAC::getDAC2()
    {
        static DAC dac = DAC(DAC_Name::DAC_2);
        return dac;
    }
}
