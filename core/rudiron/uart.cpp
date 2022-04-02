#include "uart.h"


namespace Rudiron {

    UART::UART(MDR_UART_TypeDef* MDR_UART, uint32_t RST_CLK_PCLK_UART, PortPinName RX_PIN, PORT_InitTypeDef RX_PortInit, PortPinName TX_PIN, PORT_InitTypeDef TX_PortInit): Stream(){
        this->MDR_UART = MDR_UART;
        this->RST_CLK_PCLK_UART = RST_CLK_PCLK_UART;
        this->RX_PIN = RX_PIN;
        this->RX_PortInit = RX_PortInit;
        this->TX_PIN = TX_PIN;
        this->TX_PortInit = TX_PortInit;
    }


    bool UART::begin(uint32_t baudRate) {
        /* Enables the CPU_CLK clock*/
        RST_CLK_PCLKcmd(RST_CLK_PCLK_UART, ENABLE);

        /* Set the HCLK division factor*/
        UART_BRGInit(MDR_UART, UART_HCLKdiv1);

        /* Initialize UART_InitStructure */
        UART_InitTypeDef UART_InitStructure;
        UART_InitStructure.UART_BitRate = baudRate;
        UART_InitStructure.UART_WordLength = UART_WordLength8b;
        UART_InitStructure.UART_StopBits = UART_StopBits1;
        UART_InitStructure.UART_Parity = UART_Parity_No;
        UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
        UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

        /* Configure UART parameters*/
        bool status = UART_Init(MDR_UART, &UART_InitStructure);

        if (status) {
            /* Enables UART peripheral */
            UART_Cmd(MDR_UART, ENABLE);
        }

        return status;
    }


    void UART::end() {
        UART_Cmd(MDR_UART, DISABLE);
        RST_CLK_PCLKcmd(RST_CLK_PCLK_UART, DISABLE);
    }


    int UART::available() {
        /* Check RXFF flag*/
        return UART_GetFlagStatus(MDR_UART, UART_FLAG_RXFF);
    }


    int UART::peek() {
        return read();
    }


    int UART::read() {
        /* Check RXFF flag*/
        if (UART_GetFlagStatus(MDR_UART, UART_FLAG_RXFF) != SET) {
            return -1;
        }

        /* Recive data*/
        return UART_ReceiveData(MDR_UART);
    }


    int UART::availableForWrite() {
        return UART_GetFlagStatus(MDR_UART, UART_FLAG_TXFE);
    }


    void UART::flush() {
        while (UART_GetFlagStatus(MDR_UART, UART_FLAG_TXFE) != SET) {}
    }


    size_t UART::write(uint8_t byte) {
        /* Check TXFE flag*/
        while (UART_GetFlagStatus(MDR_UART, UART_FLAG_TXFE) != SET) {}

        /* Send Data */
        UART_SendData(MDR_UART, (uint16_t) byte);

        return true;
    };


    
        // /* Fill PortInit structure*/
        // PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        // PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        // PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        // PortInit.PORT_PD = PORT_PD_DRIVER;
        // PortInit.PORT_GFEN = PORT_GFEN_OFF;
        // PortInit.PORT_FUNC = PORT_FUNC_ALTER;
        // PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        // PortInit.PORT_MODE = PORT_MODE_DIGITAL;

        // /* Configure PORTD pins 1 (UART2_TX) as output */
        // PortInit.PORT_OE = PORT_OE_OUT;
        // PortInit.PORT_Pin = PORT_Pin_1;
        // PORT_Init(MDR_PORTD, &PortInit);

        // /* Configure PORTD pins 0 (UART1_RX) as input */
        // PortInit.PORT_OE = PORT_OE_IN;
        // PortInit.PORT_Pin = PORT_Pin_0;
        // PORT_Init(MDR_PORTD, &PortInit);
    

}