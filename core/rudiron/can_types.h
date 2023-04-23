#ifndef CAN_TYPES_H
#define CAN_TYPES_H

#include "MDR32Fx.h"

namespace Rudiron
{
    typedef enum
    {
        CAN_None = -1,
        CAN_1 = 0,
        CAN_2,
    } CAN_Name;

    // Пакет CAN
    typedef struct
    {
        uint32_t ID;
        uint8_t Data[8];
        uint8_t Mask;
    } CAN_RX_Package;
}

#endif // CAN_TYPES_H
