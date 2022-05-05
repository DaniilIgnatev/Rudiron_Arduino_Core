#include "uart.h"
#include "clk.h"


#define IRQ_ENABLED UART_IRQn != IRQn::SysTick_IRQn

namespace Rudiron {

    UART::UART(
        MDR_UART_TypeDef* MDR_UART,
        uint32_t RST_CLK_PCLK_UART,
        PortPinName RX_PIN,
        PORT_InitTypeDef RX_PortInit,
        PortPinName TX_PIN,
        PORT_InitTypeDef TX_PortInit,
        IRQn_Type UART_IRQn,
        tx_buffer_index_t *_rx_buffer_head,
        tx_buffer_index_t *_rx_buffer_tail,
        int *_rx_buffer
        ): Stream(){
        this->MDR_UART = MDR_UART;
        this->RST_CLK_PCLK_UART = RST_CLK_PCLK_UART;
        this->RX_PIN = RX_PIN;
        this->RX_PortInit = RX_PortInit;
        this->TX_PIN = TX_PIN;
        this->TX_PortInit = TX_PortInit;
        this->UART_IRQn = UART_IRQn;
        this->_rx_buffer_head = _rx_buffer_head;
        this->_rx_buffer_tail = _rx_buffer_tail;
        this->_rx_buffer = _rx_buffer;
    }


    bool UART::begin(
            uint32_t baudRate,
            uint16_t UART_WordLength,
            uint16_t UART_Parity,
            uint16_t UART_StopBits
            ){

        GPIO::configPin(RX_PIN, RX_PortInit);
        GPIO::configPin(TX_PIN, TX_PortInit);

        /* Enables the CPU_CLK clock*/
        RST_CLK_PCLKcmd(RST_CLK_PCLK_UART, ENABLE);

        /* Set the HCLK division factor*/
        UART_BRGInit(MDR_UART, CLK::getHCLKdiv());

        //EnableIRQ
        if (IRQ_ENABLED){
            for (tx_buffer_index_t i = 0; i < SERIAL_RX_BUFFER_SIZE; i++){
                _rx_buffer[i] = EndOfStream;
            }

            NVIC_EnableIRQ(UART_IRQn);
            UART_ITConfig(MDR_UART, UART_IT_RX, ENABLE);
        }

        /* Initialize UART_InitStructure */
        UART_InitTypeDef UART_InitStructure;
        UART_InitStructure.UART_BitRate = baudRate;
        UART_InitStructure.UART_WordLength = UART_WordLength;
        UART_InitStructure.UART_StopBits = UART_StopBits;
        UART_InitStructure.UART_Parity = UART_Parity;
        UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
        UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

        /* Configure UART parameters*/
        bool status = UART_Init(MDR_UART, &UART_InitStructure);

        if (status) {
            UART_Cmd(MDR_UART, ENABLE);
        }

        return status;
    }


    void UART::end() {
        UART_Cmd(MDR_UART, DISABLE);
        if (IRQ_ENABLED){
            NVIC_DisableIRQ(UART_IRQn);
            UART_ITConfig(MDR_UART, UART_IT_RX, DISABLE);
        }
        RST_CLK_PCLKcmd(RST_CLK_PCLK_UART, DISABLE);
    }


    int UART::available() {
        if (IRQ_ENABLED){
            return _rx_buffer[*_rx_buffer_tail] != EndOfStream;
        }
        else{
            return UART_GetFlagStatus(MDR_UART, UART_FLAG_RXFF);
        }
    }


    int UART::peek() {
        if (IRQ_ENABLED){
            return _rx_buffer[*_rx_buffer_tail];
        }
        else{
            if (UART_GetFlagStatus(MDR_UART, UART_FLAG_RXFF) != SET) {
                return EndOfStream;
            }

            return UART_ReceiveData(MDR_UART);
        }
    }


    int UART::read() {
        if (IRQ_ENABLED){
            int data = _rx_buffer[*_rx_buffer_tail];
            if (data != EndOfStream){
                _rx_buffer[*_rx_buffer_tail] = EndOfStream;
                
                //_rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
                (*_rx_buffer_tail)++;
                if (*_rx_buffer_tail == SERIAL_RX_BUFFER_SIZE){
                    *_rx_buffer_tail = 0;
                }
            }

            return data;
        }
        else{
            if (UART_GetFlagStatus(MDR_UART, UART_FLAG_RXFF) != SET) {
                return EndOfStream;
            }

            return UART_ReceiveData(MDR_UART);
        }
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


    UART* UART::getUART1(){
        PORT_InitTypeDef RX_PortInit;
        RX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        RX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        RX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        RX_PortInit.PORT_PD = PORT_PD_DRIVER;
        RX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        RX_PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
        RX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        RX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        RX_PortInit.PORT_OE = PORT_OE_IN;

        PORT_InitTypeDef TX_PortInit;
        TX_PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
        TX_PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        TX_PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
        TX_PortInit.PORT_PD = PORT_PD_DRIVER;
        TX_PortInit.PORT_GFEN = PORT_GFEN_OFF;
        TX_PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
        TX_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
        TX_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
        TX_PortInit.PORT_OE = PORT_OE_OUT;

        static UART uart(MDR_UART1, RST_CLK_PCLK_UART1, PORT_PIN_A6, RX_PortInit, PORT_PIN_A7, TX_PortInit, UART1_IRQn, &_uart1_rx_buffer_head, &_uart1_rx_buffer_tail, _uart1_rx_buffer);
        return &uart;
    }


    UART* UART::getUART2(){
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

        static UART uart(MDR_UART2, RST_CLK_PCLK_UART2, PORT_PIN_D0, RX_PortInit, PORT_PIN_D1, TX_PortInit, UART2_IRQn, &_uart2_rx_buffer_head, &_uart2_rx_buffer_tail, _uart2_rx_buffer);
        return &uart;
    }
}