#include "adc_utility.h"

namespace Rudiron
{
    ADC_ChannelName ADC_Utility::getChannelName(PortPinName pinName)
    {
        return (ADC_ChannelName)(pinName - PORT_PIN_D0);
    }
}
