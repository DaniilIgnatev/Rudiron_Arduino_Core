#include "spi.h"

namespace Rudiron {

    void SPI::InitSPIController(uint32_t SSP_Mode) {
        //Сброс настроек контроллера SPI1
        SSP_DeInit(MDR_SSP1);
        //Установка делителя тактовой частоты SPI1
        SSP_BRGInit(MDR_SSP1, SSP_HCLKdiv1);

        /* SSP1 MASTER configuration ------------------------------------------------*/
        SSP_StructInit(&sSSP);//значения по умолчанию
        sSSP.SSP_SCR = 0x0;//коэффициент скорости обмена
        sSSP.SSP_CPSDVSR = 12;//делитель частоты
        //частота SPI = 48 / 12 (1 + 0) = 4Мгц
        //частота SPI = 80 / 12 (1 + 0) = 6.(6)Мгц

        sSSP.SSP_Mode = SSP_Mode;
        sSSP.SSP_WordLength = SSP_WordLength8b;
        sSSP.SSP_SPH = SSP_SPH_1Edge;
        sSSP.SSP_SPO = SSP_SPO_Low;
        sSSP.SSP_FRF = SSP_FRF_SPI_Motorola;
        sSSP.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;
        SSP_Init(MDR_SSP1, &sSSP);

        /* Enable SSP1 */
        SSP_Cmd(MDR_SSP1, ENABLE);
    }


    void SPI::InitSPIPortMaster() {
        //Конфигурация пинов SPI1
        PORT_InitStructure.PORT_FUNC = PORT_FUNC_ALTER;
        PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
        PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;

        PORT_InitStructure.PORT_OE = PORT_OE_IN;
        PORT_InitStructure.PORT_Pin = PORT_Pin_3;
        PORT_Init(MDR_PORTF, &PORT_InitStructure);


        PORT_InitStructure.PORT_OE = PORT_OE_OUT;
        PORT_InitStructure.PORT_Pin = PORT_Pin_0 | PORT_Pin_1;
        PORT_Init(MDR_PORTF, &PORT_InitStructure);
    }


    bool SPI::begin(uint32_t SSP_Mode) {
        InitSPIController(SSP_Mode);
        if (SSP_Mode == SSP_ModeMaster) {
            InitSPIPortMaster();
        }

        return true;
    }


    void SPI::end() {

    }


    int SPI::available() {
        return true;
    }


    int SPI::peek() {
        return read();
    }


    int SPI::read() {
        return read_write(this->NOP);
    }


    int SPI::availableForWrite() {
        return SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_TFE);
    }


    void SPI::flush() {
        while (SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_TFE) == RESET);
    }


    size_t SPI::write(uint8_t byte) {
        read_write(byte);
        return true;
    }


    uint8_t SPI::read_write(uint8_t data) {
        //Пока буфер FIFO передатчика не пуст
        while (SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_TFE) == RESET) {}
        //Отправка данных
        SSP_SendData(MDR_SSP1, data);

        //Пока буфер FIFO приемника пуст
        while (SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_RNE) == RESET) {}
        return (uint8_t) SSP_ReceiveData(MDR_SSP1);
    }

}