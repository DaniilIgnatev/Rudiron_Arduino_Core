#include "dac.h"

#include "MDR_config.h"
#include "MDR32Fx.h"
#include "MDR_port.h"
#include "MDR_rst_clk.h"
#include "MDR_dac.h"
#include "MDR_dma.h"
#include "MDR_config.h"

namespace Rudiron
{
    void DAC::enable()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK_DAC, ENABLE);
    }

    void DAC::disable()
    {
        RST_CLK_PCLKcmd(RST_CLK_PCLK_DAC, DISABLE);
    }

    void DAC::initPinDAC(PortPinName pinName)
    {
        PORT_InitTypeDef PORT_InitStructure;
        uint16_t pin = GPIO::getPinNumber(pinName);

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

    void DAC::write_pin(PortPinName pinName, uint16_t value)
    {
        DAC_ChannelName channel = DAC_Utility::getChannelName(pinName);

        
    }
}
