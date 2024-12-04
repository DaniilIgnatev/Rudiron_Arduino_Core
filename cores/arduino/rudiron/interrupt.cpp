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

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

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
