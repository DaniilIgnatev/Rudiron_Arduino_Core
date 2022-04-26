#include "can.h"

namespace Rudiron
{

    CAN::CAN(
        MDR_CAN_TypeDef *MDR_CAN,
        uint32_t RST_CLK_PCLK_CAN,
        PortPinName RX_PIN,
        PORT_InitTypeDef RX_PortInit,
        PortPinName TX_PIN,
        PORT_InitTypeDef TX_PortInit)
    {
        this->MDR_CAN = MDR_CAN;
        this->RST_CLK_PCLK_CAN = RST_CLK_PCLK_CAN;
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

        CAN_ITConfig(MDR_CAN, CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_TXINTEN | CAN_IT_ERRINTEN | CAN_IT_ERROVERINTEN, ENABLE);
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

    void CAN::setActiveID(uint32_t ID)
    {
        this->active_ID = ID;
    }

    void CAN::write(CAN_TxMsgTypeDef &TxMsg)
    {
        /* transmit */
        // TxMsg.IDE = CAN_ID_STD;
        // TxMsg.DLC = 0x08;
        // TxMsg.PRIOR_0 = DISABLE;
        // TxMsg.ID = 0x19ABFFFF;
        // TxMsg.Data[1] = data[1];
        // TxMsg.Data[0] = data[0];

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
            if (_can_rx_buffer[i].ID == this->active_ID){
                return true;
            }
        }
        
        return false;
    }

    int CAN::peek(void)
    {
        for (uint8_t i = 0; i < CAN_RX_BUFFER_SIZE; i++){
            if (_can_rx_buffer[i].ID == this->active_ID){
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
            if (_can_rx_buffer[i].ID == this->active_ID){
                for (uint8_t j = 0; j < 8; j++){
                    if ((1 << j) & _can_rx_buffer[i].Mask != 0){
                        _can_rx_buffer[i].Mask &= ~(1 << j);

                        if (_can_rx_buffer[i].Mask == 0){
                            _can_rx_buffer->ID = 0;
                        }

                        return _can_rx_buffer[i].Data[j];
                    }
                }
            }
        }

        return -1;
    }

    size_t CAN::write(uint8_t byte)
    {
    }
}