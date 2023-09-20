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
