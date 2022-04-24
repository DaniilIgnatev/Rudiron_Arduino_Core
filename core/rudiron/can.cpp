#include "can.h"


namespace Rudiron {

    CAN::CAN(
            MDR_CAN_TypeDef* MDR_CAN,
            uint32_t RST_CLK_PCLK_CAN,
            PortPinName RX_PIN,
            PORT_InitTypeDef RX_PortInit,
            PortPinName TX_PIN,
            PORT_InitTypeDef TX_PortInit
    ){
        this->MDR_CAN = MDR_CAN;
        this->RST_CLK_PCLK_CAN = RST_CLK_PCLK_CAN;
        this->RX_PIN = RX_PIN;
        this->RX_PortInit = RX_PortInit;
        this->TX_PIN = TX_PIN;
        this->TX_PortInit = TX_PortInit;
    }

    //ПЕРЕДЕЛАТЬ!
    bool CAN::begin() {
        PORT_InitTypeDef PortInit;

        /* Fill PortInit structure*/
        PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        PortInit.PORT_PD = PORT_PD_DRIVER;
        PortInit.PORT_GFEN = PORT_GFEN_OFF;
        PortInit.PORT_FUNC = PORT_FUNC_ALTER;
        PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        /* Configure PORTA pins 7 (CAN2_RX) as input */
        PortInit.PORT_OE = PORT_OE_IN;
        PortInit.PORT_Pin = PORT_Pin_7;
        PORT_Init(MDR_PORTA, &PortInit);
        /* Configure PORTA pins 6 (CAN2_TX) as output */
        PortInit.PORT_OE = PORT_OE_OUT;
        PortInit.PORT_Pin = PORT_Pin_6;
        PORT_Init(MDR_PORTA, &PortInit);


        RST_CLK_PCLKcmd((this->RST_CLK_PCLK_CAN), ENABLE);
        CAN_BRGInit(MDR_CAN, CAN_HCLKdiv1);

        /* CAN register init */
        CAN_DeInit(MDR_CAN);

        /* CAN cell init */
        CAN_InitTypeDef sCAN;
        CAN_StructInit(&sCAN);
        sCAN.CAN_ROP = DISABLE;
        sCAN.CAN_SAP = DISABLE;
        sCAN.CAN_STM = DISABLE;
        sCAN.CAN_ROM = DISABLE;

        //Normal bit time = sync segment + propagation segment + phase segment1 + phase segment2
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
        sCAN.CAN_BRP = 39;//f=500000 bit/s (T=2us)
        CAN_Init(MDR_CAN, &sCAN);

        CAN_Cmd(MDR_CAN, ENABLE);

        CAN_ITConfig(MDR_CAN, CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_TXINTEN |
                               CAN_IT_ERRINTEN | CAN_IT_ERROVERINTEN, ENABLE);

        /* Set mask and filter*/
//    CAN_FilterInitTypeDef fCAN;
//    fCAN.Mask_ID = 0;
//    fCAN.Filter_ID = 0;
//    CAN_FilterInit(MDR_CAN1, rx_buf_number, &fCAN);

        CAN_RxITConfig(MDR_CAN, 1 << rx_buf_number, ENABLE);
        CAN_TxITConfig(MDR_CAN, 1 << tx_buf_number, ENABLE);

        /* receive buffer enable */
        CAN_Receive(MDR_CAN, rx_buf_number, DISABLE);

        return true;
    }


    void CAN::end() {

    }


    void CAN::write(CAN_DataTypeDef &data) {
        CAN_TxMsgTypeDef TxMsg;

        /* transmit */
        TxMsg.IDE = CAN_ID_STD;
        TxMsg.DLC = 0x08;
        TxMsg.PRIOR_0 = DISABLE;
        TxMsg.ID = 0x19ABFFFF;
        TxMsg.Data[1] = data[1];
        TxMsg.Data[0] = data[0];

        CAN_Transmit(MDR_CAN, tx_buf_number, &TxMsg);

        uint32_t i = 0;
        while (((CAN_GetStatus(MDR_CAN) & CAN_STATUS_TX_READY) != RESET) && (i != 0xFFF)) {
            i++;
        }
        CAN_ITClearRxTxPendingBit(MDR_CAN, tx_buf_number, CAN_STATUS_TX_READY);
    }


    bool CAN::read(CAN_RxMsgTypeDef &data, bool wait, uint32_t timeout) {
        if (wait) {
            uint32_t i = 0;
            while (((CAN_GetStatus(MDR_CAN) & CAN_STATUS_RX_READY) == RESET) && (i != timeout)) {
                i++;
            }
            if (i == timeout) {
                return false;
            }
        } else if ((CAN_GetStatus(MDR_CAN) & CAN_STATUS_RX_READY) == RESET) {
            return false;
        }

        CAN_GetRawReceivedData(MDR_CAN, rx_buf_number, &data);
        CAN_ITClearRxTxPendingBit(MDR_CAN, rx_buf_number, CAN_STATUS_RX_READY);
        return true;
    }
}