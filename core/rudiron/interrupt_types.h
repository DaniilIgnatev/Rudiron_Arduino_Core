#ifndef INTERRUPT_TYPES_H
#define INTERRUPT_TYPES_H

namespace Rudiron
{
    /// @brief Внешние прерывания
    typedef enum
    {
        Interrupt_None = -1,
        EXT_INT1_Interrupt,
        EXT_INT2_Interrupt,
        EXT_INT3_Interrupt,
        EXT_INT4_Interrupt
    } Interrupt_Name;
}

#endif // INTERRUPT_TYPES_H
