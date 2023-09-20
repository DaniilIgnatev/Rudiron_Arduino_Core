/*
This file is part of Arduino_Core_Rudiron.

Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SSP_H
#define SSP_H

#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_ssp.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"

#include "Stream.h"
#include "rudiron/gpio.h"

namespace Rudiron
{
    /// Не использовать с переменными типа auto, будет ошибка
    class SSP : public Stream
    {
    protected:
        SSP_InitTypeDef SSP_InitStructure;

        MDR_SSP_TypeDef *MDR_SSP;

        uint32_t RST_CLK_PCLK_SSP;

        uint32_t SSP_Mode;

        void InitSSPPortMaster();

    public:
        explicit SSP(MDR_SSP_TypeDef *MDR_SSP, uint32_t RST_CLK_PCLK_SSP, uint16_t SSP_Mode);

        // Посылается в шину для получения данных (No operation)
        uint8_t NOP = (uint8_t)0xFF;

        /*
            Пример настроек:
            uint32_t speed = 4000000, uint16_t SSP_SPH = SSP_SPH_1Edge, uint16_t SSP_SPO = SSP_SPO_Low,
            uint16_t SSP_WordLength = SSP_WordLength8b, uint16_t SSP_FRF = SSP_FRF_SPI_Motorola, uint16_t SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE
        */
        bool begin(
            uint32_t speed, uint16_t SSP_SPH, uint16_t SSP_SPO,
            uint16_t SSP_WordLength, uint16_t SSP_FRF, uint16_t SSP_HardwareFlowControl);

        void end();

        uint8_t read_write(uint8_t data);

        uint16_t read_write16(uint16_t data);

        virtual int available(void);

        virtual int peek(void);

        virtual int read(void);

        int availableForWrite();

        void flush();

        virtual size_t write(uint8_t byte);

        using Print::write;

        operator bool() { return true; }

        static SSP &getSSP1();

        static SSP &getSSP2();
    };
}

#endif // SSP_H
