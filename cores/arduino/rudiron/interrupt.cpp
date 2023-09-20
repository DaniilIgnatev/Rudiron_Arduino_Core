#include "interrupt.h"
#include "config.h"

namespace Rudiron
{
    void Interrupt::attach(Interrupt_Name interrupt, void (*interrupt_callback)(void))
    {
        switch (interrupt)
        {
        case Interrupt_Name::EXT_INT1_Interrupt:
            NVIC_ClearPendingIRQ(IRQn_Type::EXT_INT1_IRQn);
            NVIC_EnableIRQ(IRQn_Type::EXT_INT1_IRQn);
            interrupt_callback_EXT_INT1_IRQHandler = interrupt_callback;
            break;
        case Interrupt_Name::EXT_INT2_Interrupt:
            NVIC_ClearPendingIRQ(IRQn_Type::EXT_INT2_IRQn);
            NVIC_EnableIRQ(IRQn_Type::EXT_INT2_IRQn);
            interrupt_callback_EXT_INT2_IRQHandler = interrupt_callback;
            break;
        case Interrupt_Name::EXT_INT3_Interrupt:
            NVIC_ClearPendingIRQ(IRQn_Type::EXT_INT3_IRQn);
            NVIC_EnableIRQ(IRQn_Type::EXT_INT3_IRQn);
            interrupt_callback_EXT_INT3_IRQHandler = interrupt_callback;
            break;
        case Interrupt_Name::EXT_INT4_Interrupt:
            NVIC_ClearPendingIRQ(IRQn_Type::EXT_INT4_IRQn);
            NVIC_EnableIRQ(IRQn_Type::EXT_INT4_IRQn);
            interrupt_callback_EXT_INT4_IRQHandler = interrupt_callback;
            break;
        }
    }

    void Interrupt::detach(Interrupt_Name interrupt)
    {
        switch (interrupt)
        {
        case Interrupt_Name::EXT_INT1_Interrupt:
            NVIC_ClearPendingIRQ(IRQn_Type::EXT_INT1_IRQn);
            NVIC_DisableIRQ(IRQn_Type::EXT_INT1_IRQn);
            interrupt_callback_EXT_INT1_IRQHandler = nullptr;
            break;
        case Interrupt_Name::EXT_INT2_Interrupt:
            NVIC_ClearPendingIRQ(IRQn_Type::EXT_INT2_IRQn);
            NVIC_DisableIRQ(IRQn_Type::EXT_INT2_IRQn);
            interrupt_callback_EXT_INT2_IRQHandler = nullptr;
            break;
        case Interrupt_Name::EXT_INT3_Interrupt:
            NVIC_ClearPendingIRQ(IRQn_Type::EXT_INT3_IRQn);
            NVIC_DisableIRQ(IRQn_Type::EXT_INT3_IRQn);
            interrupt_callback_EXT_INT3_IRQHandler = nullptr;
            break;
        case Interrupt_Name::EXT_INT4_Interrupt:
            NVIC_ClearPendingIRQ(IRQn_Type::EXT_INT4_IRQn);
            NVIC_DisableIRQ(IRQn_Type::EXT_INT4_IRQn);
            interrupt_callback_EXT_INT4_IRQHandler = nullptr;
            break;
        }
    }
}