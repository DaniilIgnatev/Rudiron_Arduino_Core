#ifndef DAC_TYPES_H
#define DAC_TYPES_H

namespace Rudiron
{
    // Блоки ЦАП
    typedef enum
    {
        DAC_None = -1,
        DAC_1 = 0,
        DAC_2,
    } DAC_Name;

    // Каналы цап
    typedef enum
    {
        DAC_Channel_None = -1,
        DAC_Channel_1 = 0,
        DAC_Channel_2,
    } DAC_ChannelName;
}

#endif // DAC_TYPES_H
