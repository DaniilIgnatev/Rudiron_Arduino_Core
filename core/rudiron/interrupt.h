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

#endif // INTERRUPT_H