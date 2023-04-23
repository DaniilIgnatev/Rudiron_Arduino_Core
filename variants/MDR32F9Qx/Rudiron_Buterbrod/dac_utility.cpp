#include "dac_utility.h"

namespace Rudiron
{
    DAC_ChannelName DAC_Utility::getDAC_ChannelName(PortPinName pinName)
    {
        switch (pinName)
        {
        case PORT_PIN_E0:
            return DAC_ChannelName::DAC_Channel_2;
        default:
            return DAC_ChannelName::DAC_Channel_None;
        }
    }

    DAC_Name DAC_Utility::getDAC_Name(DAC_ChannelName channelName)
    {
        switch (channelName)
        {
        case DAC_Channel_2:
            return DAC_Name::DAC_2;
        default:
            return DAC_Name::DAC_None;
        }
    }
}
