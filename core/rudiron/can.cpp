#include "can.h"

namespace Rudiron
{

    CAN::CAN(
        MDR_CAN_TypeDef *MDR_CAN,
        uint32_t RST_CLK_PCLK_CAN,
        IRQn CAN_IRQn,
        PortPinName RX_PIN,
        PORT_InitTypeDef RX_PortInit,
        PortPinName TX_PIN,
        PORT_InitTypeDef TX_PortInit)
    {
        this->MDR_CAN = MDR_CAN;
        this->RST_CLK_PCLK_CAN = RST_CLK_PCLK_CAN;
        this->CAN_IRQn = CAN_IRQn;
        this->RX_PIN = RX_PIN;
        this->RX_PortInit = RX_PortInit;
        this->TX_PIN = TX_PIN;
        this->TX_PortInit = TX_PortInit;
    }

    bool CAN::begin(uint32_t baudRate)
    {
        GPIO::configPin(RX_PIN, RX_PortInit);
        GPIO::configPin(TX_PIN, TX_PortInit);

        RST_CLK_PCLKcmd(this->RST_CLK_PCLK_CAN, ENABLE);
        CAN_BRGInit(MDR_CAN, CAN_HCLKdiv1);

        //Задание скорости передачи и момента семплирования
        CAN_DeInit(MDR_CAN);

        CAN_InitTypeDef sCAN;
        CAN_StructInit(&sCAN);
        sCAN.CAN_ROP = DISABLE;
        sCAN.CAN_SAP = DISABLE;
        sCAN.CAN_STM = DISABLE;
        sCAN.CAN_ROM = DISABLE;

        // Normal bit time = sync segment + propagation segment + phase segment1 + phase segment2
        //    sCAN.CAN_PSEG = CAN_PSEG_Mul_2TQ;
        //    sCAN.CAN_SEG1 = CAN_SEG1_Mul_5TQ;
        //    sCAN.CAN_SEG2 = CAN_SEG2_Mul_5TQ;
        //    sCAN.CAN_SJW = CAN_SJW_Mul_4TQ;
        //    sCAN.CAN_SB = CAN_SB_3_SAMPLE;
        //    sCAN.CAN_BRP = 4;//

        //    sCAN.CAN_PSEG = CAN_PSEG_Mul_1TQ;
        //    sCAN.CAN_SEG1 = CAN_SEG1_Mul_2TQ;
        //    sCAN.CAN_SEG2 = CAN_SEG2_Mul_2TQ;
        //    sCAN.CAN_SJW = CAN_SJW_Mul_4TQ;
        //    sCAN.CAN_SB = CAN_SB_3_SAMPLE;
        //    sCAN.CAN_BRP = 47;//f=277777 bit/s (T=3.6us)

        sCAN.CAN_PSEG = CAN_PSEG_Mul_1TQ;
        sCAN.CAN_SEG1 = CAN_SEG1_Mul_1TQ;
        sCAN.CAN_SEG2 = CAN_SEG2_Mul_1TQ;
        sCAN.CAN_SJW = CAN_SJW_Mul_4TQ;
        sCAN.CAN_SB = CAN_SB_3_SAMPLE;
        sCAN.CAN_BRP = 39; // f=500000 bit/s (T=2us)
        CAN_Init(MDR_CAN, &sCAN);

        NVIC_EnableIRQ(CAN_IRQn);

        CAN_ITConfig(MDR_CAN, CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_ERROVERINTEN, ENABLE);
        CAN_Cmd(MDR_CAN, ENABLE);

        // configure buffer read
        CAN_FilterInitTypeDef fCAN;
        fCAN.Mask_ID = 0;
        fCAN.Filter_ID = 0;
        CAN_FilterInit(MDR_CAN, 1, &fCAN);

        CAN_RxITConfig(MDR_CAN, 1 << 1, ENABLE);
        CAN_Receive(MDR_CAN, 1, DISABLE);

        // configure buffer write
        CAN_TxITConfig(MDR_CAN, 1 << 0, ENABLE);

        return true;
    }

    void CAN::end()
    {
        CAN_Cmd(MDR_CAN, DISABLE);
        CAN_ITConfig(MDR_CAN, CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_TXINTEN | CAN_IT_ERRINTEN | CAN_IT_ERROVERINTEN, DISABLE);
    }

    bool CAN::setActiveID(uint32_t standart_part, uint32_t extended_part)
    {
        uint32_t t_st_ID = standart_part & 0b00000000000000000000011111111111;
        //7 старших бит не должны быть все единичными битами
        if (((t_st_ID & 0b00000000000000000000011111110000) ^ 0b00000000000000000000011111110000) == 0){
            return false;
        }

        activeID_extended = (bool)extended_part;
        uint32_t t_ext_ID = extended_part & 0b000000000000000000000111111111111111111;
        this->activeID = (t_st_ID << 18) | t_ext_ID;

        return true;
    }

    bool CAN::setActiveID(uint32_t extendedID){
        uint32_t t_ext_ID = extendedID & 0b00011111111111111111111111111111;

        //7 старших бит не должны быть все единичными битами
        if (((t_ext_ID & 0b00011111110000000000000000000000) ^ 0b00011111110000000000000000000000) == 0){
            return false;
        }

        activeID_extended = true;
        this->activeID = extendedID;
        return true;
    }

    void CAN::write(CAN_TxMsgTypeDef &TxMsg)
    {
        CAN_Transmit(MDR_CAN, 0, &TxMsg);

        uint32_t i = 0;
        while (((CAN_GetStatus(MDR_CAN) & CAN_STATUS_TX_READY) != RESET) && (i != 0xFFF))
        {
            i++;
        }
        CAN_ITClearRxTxPendingBit(MDR_CAN, 0, CAN_STATUS_TX_READY);
    }

    int CAN::available(void)
    {
        for (uint8_t i = 0; i < CAN_RX_BUFFER_SIZE; i++){
            if (_can_rx_buffer[i].ID == this->activeID){
                return true;
            }
        }
        
        return false;
    }

    int CAN::peek(void)
    {
        for (uint8_t i = 0; i < CAN_RX_BUFFER_SIZE; i++){
            if (_can_rx_buffer[i].ID == this->activeID){
                for (uint8_t j = 0; j < 8; j++){
                    if ((1 << j) & _can_rx_buffer[i].Mask != 0){
                        return _can_rx_buffer[i].Data[j];
                    }
                }
            }
        }

        return -1;
    }

    int CAN::read(void)
    {
        for (uint8_t i = 0; i < CAN_RX_BUFFER_SIZE; i++){
            if (_can_rx_buffer[i].ID == this->activeID){
                for (uint8_t j = 0; j < 8; j++){
                    if (((1 << j) & _can_rx_buffer[i].Mask) != 0){
                        _can_rx_buffer[i].Mask &= ~(1 << j);

                        if (_can_rx_buffer[i].Mask == 0){
                            _can_rx_buffer[i].ID = 0;
                        }

                        uint8_t result = _can_rx_buffer[i].Data[j];
                        _can_rx_buffer[i].Data[j] = 0;
                        return result;
                    }
                }
            }
        }

        return -1;
    }

    size_t CAN::write(uint8_t byte)
    {
        CAN_TxMsgTypeDef message;
        message.IDE = activeID_extended ? CAN_ID_EXT : CAN_ID_STD;
        message.DLC = 0x01;
        message.PRIOR_0 = DISABLE;
        message.ID = activeID;
        message.Data[1] = 0;
        message.Data[0] = byte;
        write(message);
    }

    CAN* CAN::getCAN1(){
        PORT_InitTypeDef RX_PortInit;
        RX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        RX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        RX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        RX_PortInit.PORT_PD = PORT_PD_DRIVER;
        RX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        RX_PortInit.PORT_FUNC = PORT_FUNC_ALTER;
        RX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        RX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        RX_PortInit.PORT_OE = PORT_OE_IN;

        PORT_InitTypeDef TX_PortInit;
        TX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        TX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        TX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        TX_PortInit.PORT_PD = PORT_PD_DRIVER;
        TX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        TX_PortInit.PORT_FUNC = PORT_FUNC_ALTER;
        TX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        TX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        TX_PortInit.PORT_OE = PORT_OE_OUT;

        static CAN can(MDR_CAN1, RST_CLK_PCLK_CAN1, IRQn::CAN1_IRQn, PORT_PIN_A7, RX_PortInit, PORT_PIN_A6, TX_PortInit);
        return &can;
    }

    CAN* CAN::getCAN2(){
        PORT_InitTypeDef RX_PortInit;
        RX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        RX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        RX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        RX_PortInit.PORT_PD = PORT_PD_DRIVER;
        RX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        RX_PortInit.PORT_FUNC = PORT_FUNC_ALTER;
        RX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        RX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        RX_PortInit.PORT_OE = PORT_OE_IN;

        PORT_InitTypeDef TX_PortInit;
        TX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        TX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        TX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        TX_PortInit.PORT_PD = PORT_PD_DRIVER;
        TX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        TX_PortInit.PORT_FUNC = PORT_FUNC_ALTER;
        TX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        TX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        TX_PortInit.PORT_OE = PORT_OE_OUT;

        static CAN can(MDR_CAN2, RST_CLK_PCLK_CAN2, IRQn::CAN2_IRQn, PORT_PIN_E6, RX_PortInit, PORT_PIN_E7, TX_PortInit);
        return &can;
    }
}