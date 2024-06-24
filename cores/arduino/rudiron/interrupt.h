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

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "rudiron/interrupt_types.h"
#include "rudiron/gpio_types.h"

namespace Rudiron
{
    /// @brief Внешние прерывания
    class Interrupt
    {
    public:
        /// @brief Разрешает внешнее прерывание и назначает ему пользовательский обработчик
        /// @param interrupt название прерывания
        /// @param interrupt_callback функция-обработчик
        static void attach(Interrupt_Name interrupt, void (*interrupt_callback)(void));

        /// @brief Запрещает внешнее прерывание и сбрасывает его обработчик
        /// @param interrupt 
        static void detach(Interrupt_Name interrupt);
    };
}

#endif // INTERRUPT_
