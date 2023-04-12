#include "dac_utility.h"

namespace Rudiron
{
    DAC_ChannelName DAC_Utility::getChannelName(PortPinName pinName)
    {
        switch (pinName)
        {
        case PORT_PIN_E0:
            return DAC_ChannelName::DAC_Channel_1;        
        default:
            return DAC_ChannelName::DAC_Channel_None;
        }
    }
}
