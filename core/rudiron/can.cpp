#include "can.h"




Can::Can()
{

}


bool Can::begin()
{
    static PORT_InitTypeDef PortInit;

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


    CAN_InitTypeDef  sCAN;
    RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_CAN1),ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA,ENABLE);
    CAN_BRGInit(MDR_CAN1,CAN_HCLKdiv1);

    /* CAN register init */
    CAN_DeInit(MDR_CAN1);

    /* CAN cell init */
    CAN_StructInit (&sCAN);

    sCAN.CAN_ROP  = ENABLE;
    sCAN.CAN_SAP  = ENABLE;
    sCAN.CAN_STM  = DISABLE;
    sCAN.CAN_ROM  = DISABLE;
    //сделать расчет частоты на 500кбс
    sCAN.CAN_PSEG = CAN_PSEG_Mul_2TQ;
    sCAN.CAN_SEG1 = CAN_SEG1_Mul_5TQ;
    sCAN.CAN_SEG2 = CAN_SEG2_Mul_5TQ;
    sCAN.CAN_SJW  = CAN_SJW_Mul_4TQ;
    sCAN.CAN_SB   = CAN_SB_3_SAMPLE;
    sCAN.CAN_BRP  =  1;


    CAN_Init (MDR_CAN1,&sCAN);

    CAN_Cmd(MDR_CAN1, ENABLE);

    /* Disable all CAN1 interrupt */
    CAN_ITConfig( MDR_CAN1, CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_TXINTEN |
                  CAN_IT_ERRINTEN | CAN_IT_ERROVERINTEN, DISABLE);

    /* Enable CAN1 interrupt from receive buffer */
    CAN_RxITConfig( MDR_CAN1 ,rx_buf, ENABLE);
    /* Enable CAN1 interrupt from transmit buffer */
    CAN_TxITConfig( MDR_CAN1 ,tx_buf, ENABLE);

    /* receive buffer enable */
    CAN_Receive(MDR_CAN1, rx_buf, DISABLE);

    return true;
}


void Can::end()
{

}


void Can::write()
{
    CAN_TxMsgTypeDef TxMsg;

    /* transmit */
    TxMsg.IDE     = CAN_ID_STD;
    TxMsg.DLC     = 0x08;
    TxMsg.PRIOR_0 = DISABLE;
    TxMsg.ID      = 0x7;
    TxMsg.Data[1] = 0x01234567;
    TxMsg.Data[0] = 0x89ABCDEF;

    CAN_Transmit(MDR_CAN1, tx_buf, &TxMsg);

    uint32_t i = 0;
    //i это таймаут
    while(((CAN_GetStatus(MDR_CAN1) & CAN_STATUS_TX_READY) != RESET) && (i != 0xFFF))
    {
        i++;
    }
    CAN_ITClearRxTxPendingBit(MDR_CAN1, tx_buf, CAN_STATUS_TX_READY);
}


void Can::read()
{
    CAN_RxMsgTypeDef RxMsg;

    uint32_t i = 0;
    //i это таймаут
    while(((CAN_GetStatus(MDR_CAN1) & CAN_STATUS_RX_READY) == RESET) && (i != 0xFFF))
    {
        i++;
    }
    CAN_GetRawReceivedData(MDR_CAN1, rx_buf, &RxMsg);

    CAN_ITClearRxTxPendingBit(MDR_CAN1, rx_buf, CAN_STATUS_RX_READY);
}
