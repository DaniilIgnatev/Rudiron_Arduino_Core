#include "spi.h"
#include "clk.h"

namespace Rudiron
{

    SPI::SPI(MDR_SSP_TypeDef *MDR_SSP, uint32_t RST_CLK_PCLK_SSP, uint16_t SSP_Mode) : Stream()
    {
        this->MDR_SSP = MDR_SSP;
        this->RST_CLK_PCLK_SSP = RST_CLK_PCLK_SSP;
        this->SSP_Mode = SSP_Mode;
    }

    bool SPI::begin(
        uint32_t speed, uint16_t SSP_SPH, uint16_t SSP_SPO,
        uint16_t SSP_WordLength, uint16_t SSP_FRF, uint16_t SSP_HardwareFlowControl)
    {
        // Сброс настроек контроллера SPI1
        SSP_DeInit(MDR_SSP);
        RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP, ENABLE);

        // Установка делителя тактовой частоты SPI1
        // Частота тактирования модуля = 16Mhz
        uint32_t div = CLK::getHCLKdiv();
        SSP_BRGInit(MDR_SSP, div);

        speed /= 1000; // частота кратна тысячи герц
        if (speed == 0)
        {
            return false;
        }

        const uint16_t SSP_CPSDVSR = 2;              // делитель частоты от 2 до 254
        const uint16_t SSP_SCR = (8000 / speed) - 1; // коэффициент скорости обмена от 1 до 255

        if (SSP_CPSDVSR < 2 || SSP_CPSDVSR > 254 || SSP_SCR > 255)
        {
            return false;
        }

        SSP_StructInit(&SPI_InitStructure); // значения по умолчанию
        SPI_InitStructure.SSP_CPSDVSR = SSP_CPSDVSR;
        SPI_InitStructure.SSP_SCR = SSP_SCR;
        SPI_InitStructure.SSP_Mode = SSP_Mode;
        SPI_InitStructure.SSP_WordLength = SSP_WordLength;
        SPI_InitStructure.SSP_SPH = SSP_SPH;
        SPI_InitStructure.SSP_SPO = SSP_SPO;
        SPI_InitStructure.SSP_FRF = SSP_FRF;
        SPI_InitStructure.SSP_HardwareFlowControl = SSP_HardwareFlowControl;
        SSP_Init(MDR_SSP, &SPI_InitStructure);

        if (SSP_Mode == SSP_ModeMaster)
        {
            InitSPIPortMaster();
        }

        SSP_Cmd(MDR_SSP, ENABLE);

        return true;
    }

    void SPI::InitSPIPortMaster()
    {
        PORT_InitTypeDef PORT_InitStructure;
        PORT_StructInit(&PORT_InitStructure);

        PORT_InitStructure.PORT_FUNC = PORT_FUNC_ALTER;
        PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
        PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;

        if (MDR_SSP == MDR_SSP2)
        {
            // Конфигурация пинов SPI2
            //  RX
            PORT_InitStructure.PORT_OE = PORT_OE_IN;
            PORT_InitStructure.PORT_Pin = PORT_Pin_2;
            GPIO::configPin(PORT_PIN_D2, PORT_InitStructure);

            // CLK, TX
            PORT_InitStructure.PORT_OE = PORT_OE_OUT;
            PORT_InitStructure.PORT_Pin = PORT_Pin_5 | PORT_Pin_6;
            GPIO::configPin(PORT_PIN_D5, PORT_InitStructure);
            GPIO::configPin(PORT_PIN_D6, PORT_InitStructure);
        }
        else
        {
            // Конфигурация пинов SPI1
            //  RX
            PORT_InitStructure.PORT_OE = PORT_OE_IN;
            PORT_InitStructure.PORT_Pin = PORT_Pin_3;
            GPIO::configPin(PORT_PIN_F3, PORT_InitStructure);

            // CLK, TX
            PORT_InitStructure.PORT_OE = PORT_OE_OUT;
            PORT_InitStructure.PORT_Pin = PORT_Pin_0 | PORT_Pin_1;
            GPIO::configPin(PORT_PIN_F0, PORT_InitStructure);
            GPIO::configPin(PORT_PIN_F1, PORT_InitStructure);
        }
    }

    void SPI::end()
    {
        // ждем отправки оставшихся пакетов
        while (SSP_GetFlagStatus(MDR_SSP, SSP_FLAG_TFE) == RESET)
        {
        }
        SSP_DeInit(MDR_SSP);
        SSP_Cmd(MDR_SSP, DISABLE);
        RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP, DISABLE);
    }

    int SPI::available()
    {
        return true;
    }

    int SPI::peek()
    {
        return read();
    }

    int SPI::read()
    {
        return read_write(this->NOP);
    }

    int SPI::availableForWrite()
    {
        return SSP_GetFlagStatus(MDR_SSP, SSP_FLAG_TFE);
    }

    void SPI::flush()
    {
        while (SSP_GetFlagStatus(MDR_SSP, SSP_FLAG_TFE) == RESET)
            ;
    }

    size_t SPI::write(uint8_t byte)
    {
        read_write(byte);
        return true;
    }

    uint8_t SPI::read_write(uint8_t data)
    {
        // Пока буфер FIFO передатчика не пуст
        while (SSP_GetFlagStatus(MDR_SSP, SSP_FLAG_TFE) == RESET)
        {
        }
        // Отправка данных
        SSP_SendData(MDR_SSP, data);

        // Пока буфер FIFO приемника пуст
        while (SSP_GetFlagStatus(MDR_SSP, SSP_FLAG_RNE) == RESET)
        {
        }
        return (uint8_t)SSP_ReceiveData(MDR_SSP);
    }

    uint16_t SPI::read_write16(uint16_t data)
    {
        // Пока буфер FIFO передатчика не пуст
        while (SSP_GetFlagStatus(MDR_SSP, SSP_FLAG_TFE) == RESET)
        {
        }
        // Отправка данных
        SSP_SendData(MDR_SSP, data);

        // Пока буфер FIFO приемника пуст
        while (SSP_GetFlagStatus(MDR_SSP, SSP_FLAG_RNE) == RESET)
        {
        }
        return (uint16_t)SSP_ReceiveData(MDR_SSP);
    }

    SPI &SPI::getSPI1()
    {
        static SPI spi(MDR_SSP1, RST_CLK_PCLK_SSP1, SSP_ModeMaster);
        return spi;
    }

    SPI &SPI::getSPI2()
    {
        static SPI spi(MDR_SSP2, RST_CLK_PCLK_SSP2, SSP_ModeMaster);
        return spi;
    }
}
