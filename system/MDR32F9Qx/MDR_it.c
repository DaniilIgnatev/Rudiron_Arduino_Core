/**
  ******************************************************************************
  * @file    Examples/MDR32F9Q3_EVAL/BKP/RTC/MDR32F9Qx_it.c
  * @author  Milandr Application Team
  * @version V1.2.0
  * @date    04/07/2011
  * @brief   Main Interrupt Service Routines.
  *
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Milandr</center></h2>
  * 
  * Modified for Arduino_Core_Rudiron by Daniil Ignatev on 08.03.2022
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR_it.h"
#include "MDR_uart.h"
#include "MDR_can.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles Debug PendSV exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

extern uint32_t _micros;
extern uint32_t _millis;

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
    _micros += 5;
    if (_micros >= 1000){
        _micros = 0;
        _millis++;
    }
}

CAN_RxMsgTypeDef can_rx;

/*******************************************************************************
* Function Name  : CAN1_IRQHandler
* Description    : This function handles CAN1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN1_IRQHandler(void)
{
    CAN_GetRawReceivedData(MDR_CAN1, 1, &can_rx);

    uint8_t *bytes_to_write_pointer = (uint8_t*)&(can_rx.Data);
    uint8_t bytes_to_write_index = 0;

    uint8_t last_used_buffer_for_id = -1;
    uint8_t last_free_buffer = -1;

    //сканирование буферов
    for (uint8_t i = CAN_RX_BUFFER_SIZE - 1; i > 0; i--){
        if (_can_rx_buffer[i].ID == can_rx.Rx_Header.ID){
            last_used_buffer_for_id = i;
            break;
        }
        else
        if (_can_rx_buffer[i].ID == 0){
            last_free_buffer = i;
        }
    }


    if (last_used_buffer_for_id != -1){
        uint8_t last_free_byte = -1;
        //поиск последнего свободного байта в использованном буфере
        for (uint8_t i = 7; i > 0; i--){
            if ((1 << i) & _can_rx_buffer[last_used_buffer_for_id].Mask){
                break;
            }
            else{
                last_free_byte = i;
            }
        }

        //запись в использованный буфер
        for (uint8_t i = last_free_byte; i < 8; i++){
            _can_rx_buffer[last_used_buffer_for_id].Data[i] = bytes_to_write_pointer[bytes_to_write_index];
            _can_rx_buffer[last_used_buffer_for_id].Mask |= (1 << i);
            bytes_to_write_index++;

            //есть ли еще байты на запись
            if (bytes_to_write_index == can_rx.Rx_Header.DLC){
                break;
            }
        }
    }


    if (last_free_buffer != -1 && (bytes_to_write_index != can_rx.Rx_Header.DLC)){
        //подготовка буфера
        _can_rx_buffer[last_free_buffer].ID = can_rx.Rx_Header.ID;
        _can_rx_buffer[last_free_buffer].Mask = 0;

        //запись оставшихся байт в свободный буфер
        uint8_t i = 0;
        do
        {
            _can_rx_buffer[last_free_buffer].Data[i] = bytes_to_write_pointer[bytes_to_write_index];
            _can_rx_buffer[last_free_buffer].Mask |= (1 << i);

            i++;
            bytes_to_write_index++;
        }
        while (bytes_to_write_index != can_rx.Rx_Header.DLC);
    }
        
    
    CAN_ITClearRxTxPendingBit(MDR_CAN1, 1, CAN_STATUS_RX_READY);
}

/*******************************************************************************
* Function Name  : CAN2_IRQHandler
* Description    : This function handles CAN2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA_IRQHandler
* Description    : This function handles DMA global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : This function handles UART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1_IRQHandler(void)
{
    if (UART_GetFlagStatus(MDR_UART1, UART_FLAG_RXFF) != SET) {
        return;
    }

    _uart1_rx_buffer[_uart1_rx_buffer_head] = (unsigned char)MDR_UART1->DR;
    _uart1_rx_buffer_head++;
    
    if (_uart1_rx_buffer_head == SERIAL_RX_BUFFER_SIZE){
        _uart1_rx_buffer_head = 0;
    }
}

/*******************************************************************************
* Function Name  : UART2_IRQHandler
* Description    : This function handles UART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART2_IRQHandler(void)
{
    if (UART_GetFlagStatus(MDR_UART2, UART_FLAG_RXFF) != SET) {
        return;
    }

    _uart2_rx_buffer[_uart2_rx_buffer_head] = (unsigned char)MDR_UART2->DR;
    _uart2_rx_buffer_head++;

    if (_uart2_rx_buffer_head == SERIAL_RX_BUFFER_SIZE){
        _uart2_rx_buffer_head = 0;
    }
}

/*******************************************************************************
* Function Name  : SSP1_IRQHandler
* Description    : This function handles SSP1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SSP1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C_IRQHandler
* Description    : This function handles I2C global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : POWER_IRQHandler
* Description    : This function handles POWER global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void POWER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : Timer1_IRQHandler
* Description    : This function handles Timer1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Timer1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : Timer2_IRQHandler
* Description    : This function handles Timer2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Timer2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : Timer3_IRQHandler
* Description    : This function handles Timer3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Timer3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : COMPARATOR_IRQHandler
* Description    : This function handles COMPARATOR global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void COMPARATOR_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SSP2_IRQHandler
* Description    : This function handles SSP2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SSP2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : BACKUP_IRQHandler
* Description    : This function handles BACKUP global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BACKUP_IRQHandler(void)
{
    if (BKP_RTC_GetFlagStatus(BKP_RTC_FLAG_SECF)==SET)
    {

    }
    if (BKP_RTC_GetFlagStatus(BKP_RTC_FLAG_ALRF)==SET)
    {

    }
    MDR_BKP -> RTC_CS |= 0x06;
}

/*******************************************************************************
* Function Name  : EXT_INT1_IRQHandler
* Description    : This function handles EXT_INT1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXT_INT1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXT_INT2_IRQHandler
* Description    : This function handles EXT_INT2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXT_INT2_IRQHandler(void)
{
    //    NVIC_ClearPendingIRQ(EXT_INT1_IRQn);

//    // Buffer to store a payload of maximum width
//    uint8_t nRF24_payload[32];

//    // Pipe number
//    nRF24_RXResult pipe;

//    // Length of received payload
//    uint8_t payload_length;
//    if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY) {
//        // Get a payload from the transceiver
//        nRF24_ClearIRQFlags();
//        nRF24_maskIRQ(0,0,0);
//        pipe = nRF24_ReadPayload(nRF24_payload, &payload_length);
//    }
}

/*******************************************************************************
* Function Name  : EXT_INT3_IRQHandler
* Description    : This function handles EXT_INT3 global interrupt request.
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXT_INT3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXT_INT4_IRQHandler
* Description    : This function handles EXT_INT4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXT_INT4_IRQHandler(void)
{
}

/******************* (C) COPYRIGHT 2011 Milandr *********/

/* END OF FILE MDR32F9Qx_it.c */
