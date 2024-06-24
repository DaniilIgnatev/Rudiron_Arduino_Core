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

#ifndef DMA_TYPES_H
#define DMA_TYPES_H

#include "MDR32F9Qx_dma.h"

namespace Rudiron
{
    typedef enum
    {
        DMA_Number_Continuous_Transfers_1 = DMA_Transfers_1,
        DMA_Number_Continuous_Transfers_2 = DMA_Transfers_2,
        DMA_Number_Continuous_Transfers_4 = DMA_Transfers_4,
        DMA_Number_Continuous_Transfers_8 = DMA_Transfers_8,
        DMA_Number_Continuous_Transfers_16 = DMA_Transfers_16,
        DMA_Number_Continuous_Transfers_32 = DMA_Transfers_32,
        DMA_Number_Continuous_Transfers_64 = DMA_Transfers_64,
        DMA_Number_Continuous_Transfers_128 = DMA_Transfers_128,
        DMA_Number_Continuous_Transfers_256 = DMA_Transfers_256,
        DMA_Number_Continuous_Transfers_512 = DMA_Transfers_512,
        DMA_Number_Continuous_Transfers_1024 = DMA_Transfers_1024,
    } DMA_Number_Continuous_Transfers;

    inline uint32_t DMA_Number_Continuous_Transfers_toNumber(DMA_Number_Continuous_Transfers transfers)
    {
        return (uint32_t)transfers;
    }
}

#endif // DMA_TYPES_H
