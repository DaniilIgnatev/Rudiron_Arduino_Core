#include "nRF24_example.h"


uint32_t i,j,k;

// Pipe number
nRF24_RXResult pipe;


// Length of received payload
uint8_t payload_length;

// Buffer to store a payload of maximum width
uint8_t nRF24_payload[32];


void nRF24_DEMO_RX_SINGLE_ESB(){

    // This is simple receiver with Enhanced ShockBurst:
    //   - RX address: 'ESB'
    //   - payload: 10 bytes
    //   - RF channel: 40 (2440MHz)
    //   - data rate: 2Mbps
    //   - CRC scheme: 2 byte

    // The transmitter sends a 10-byte packets to the address 'ESB' with Auto-ACK (ShockBurst enabled)

    // Set RF channel
    nRF24_SetRFChannel(40);

    // Set data rate
    nRF24_SetDataRate(nRF24_DR_250kbps);

    // Set CRC scheme
    nRF24_SetCRCScheme(nRF24_CRC_2byte);

    // Set address width, its common for all pipes (RX and TX)
    nRF24_SetAddrWidth(5);

    // Configure RX PIPE
    static const uint8_t nRF24_ADDR[] = { 0xF0, 0xF0, 0xF0, 0xF0, 0xF0 };
    nRF24_SetAddr(nRF24_PIPE0, nRF24_ADDR); // program address for pipe

    nRF24_SetRXPipe(nRF24_PIPE0, nRF24_AA_ON, 32); // Auto-ACK: enabled, payload length: 10 bytes
    nRF24_SetTXPower(nRF24_TXPWR_18dBm);
    // Set operational mode (PRX == receiver)
    nRF24_SetOperationalMode(nRF24_MODE_RX);
    // Clear any pending IRQ flags
    nRF24_ClearIRQFlags();
    // Wake the transceiver
    nRF24_SetPowerMode(nRF24_PWR_UP);
    // Put the transceiver to the RX mode
    nRF24_CE_H;

    // The main loop
    while (1) {

        Rudiron::CLK::delay_millis(100);
    }
}


void nRF24_DEMO_TX_SINGLE_ESB(){

    // This is simple transmitter with Enhanced ShockBurst (to one logic address):
    //   - TX address: 'ESB'
    //   - payload: 10 bytes
    //   - RF channel: 40 (2440MHz)
    //   - data rate: 2Mbps
    //   - CRC scheme: 2 byte

    // The transmitter sends a 10-byte packets to the address 'ESB' with Auto-ACK (ShockBurst enabled)

    // Set RF channel
    nRF24_SetRFChannel(40);

    // Set data rate
    nRF24_SetDataRate(nRF24_DR_250kbps);

    // Set CRC scheme
    nRF24_SetCRCScheme(nRF24_CRC_2byte);

    // Set address width, its common for all pipes (RX and TX)
    nRF24_SetAddrWidth(5);

    // Configure TX PIPE
    static const uint8_t nRF24_ADDR[] = { 0xF0, 0xF0, 0xF0, 0xF0, 0xF0 };
    nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR); // program TX address
    nRF24_SetAddr(nRF24_PIPE0, nRF24_ADDR); // program address for pipe#0, must be same as TX (for Auto-ACK)

    // Set TX power (maximum)
    nRF24_SetTXPower(nRF24_TXPWR_6dBm);

    // Configure auto retransmit: 10 retransmissions with pause of 2500s in between
    nRF24_SetAutoRetr(nRF24_ARD_2500us, 10);

    // Enable Auto-ACK for pipe#0 (for ACK packets)
    nRF24_EnableAA(nRF24_PIPE0);

    /// Set operational mode (PTX == transmitter)
    nRF24_SetOperationalMode(nRF24_MODE_TX);

    // Clear any pending IRQ flags
    nRF24_ClearIRQFlags();

    // Wake the transceiver
    nRF24_SetPowerMode(nRF24_PWR_UP);


    // Some variables
    uint32_t packets_lost = 0; // global counter of lost packets
    uint8_t otx;


    // The main loop
    payload_length = 32;
    j = 0;
    while (1) {
        // Prepare data packet
        for (i = 0; i < payload_length; i++) {
            nRF24_payload[i] = j++;
            if (j > 0x000000FF) j = 0;
        }

        // Print a payload
//        UART_SendStr("PAYLOAD:>");
//        UART_SendBufHex((char *)nRF24_payload, payload_length);
//        UART_SendStr("< ... TX: ");

        // Transmit a packet
        nRF24_TXResult tx_res = nRF24_TransmitPacket(nRF24_payload, payload_length);
        otx = nRF24_GetRetransmitCounters();
        uint8_t otx_plos_cnt = (otx & nRF24_MASK_PLOS_CNT) >> 4; // packets lost counter
        //uint8_t otx_arc_cnt  = (otx & nRF24_MASK_ARC_CNT); // auto retransmissions counter
        switch (tx_res) {
        case nRF24_TX_SUCCESS:
            //UART_SendStr("OK");
            break;
        case nRF24_TX_TIMEOUT:
            //UART_SendStr("TIMEOUT");
            break;
        case nRF24_TX_MAXRT:
            //UART_SendStr("MAX RETRANSMIT");
            packets_lost += otx_plos_cnt;
            nRF24_ResetPLOS();
            break;
        default:
            //UART_SendStr("ERROR");
            break;
        }
//        UART_SendStr("   ARC=");
//        UART_SendInt(otx_arc_cnt);
//        UART_SendStr(" LOST=");
//        UART_SendInt(packets_lost);
//        UART_SendStr("\r\n");

        // Wait ~0.5s
        Rudiron::CLK::delay_millis(500);
    }

}


void nRF24_DEMO_RX_SINGLE(){
    // This is simple receiver with one RX pipe:
    //   - pipe#1 address: '0xE7 0x1C 0xE3'
    //   - payload: 5 bytes
    //   - RF channel: 115 (2515MHz)
    //   - data rate: 250kbps (minimum possible, to increase reception reliability)
    //   - CRC scheme: 2 byte

    // The transmitter sends a 5-byte packets to the address '0xE7 0x1C 0xE3' without Auto-ACK (ShockBurst disabled)

    // Disable ShockBurst for all RX pipes
    nRF24_DisableAA(0xFF);

    // Set RF channel
    nRF24_SetRFChannel(40);

    // Set data rate
    nRF24_SetDataRate(nRF24_DR_250kbps);

    // Set CRC scheme
    nRF24_SetCRCScheme(nRF24_CRC_2byte);

    // Set address width, its common for all pipes (RX and TX)
    nRF24_SetAddrWidth(5);

    // Configure RX PIPE#1
    static const uint8_t nRF24_ADDR[] = { '2', 'N', 'o', 'd', 'e' };
    nRF24_SetAddr(nRF24_PIPE1, nRF24_ADDR); // program address for RX pipe #1
    nRF24_SetRXPipe(nRF24_PIPE1, nRF24_AA_OFF, 12); // Auto-ACK: disabled, payload length: 5 bytes

    // Set operational mode (PRX == receiver)
    nRF24_SetOperationalMode(nRF24_MODE_RX);

    // Wake the transceiver
    nRF24_SetPowerMode(nRF24_PWR_UP);

    // Put the transceiver to the RX mode
    nRF24_CE_H;


    // The main loop
    while (1) {
        //
        // Constantly poll the status of the RX FIFO and get a payload if FIFO is not empty
        //
        // This is far from best solution, but it's ok for testing purposes
        // More smart way is to use the IRQ pin :)
        //
        uint8_t status = nRF24_GetStatus_RXFIFO();
        if (status != nRF24_STATUS_RXFIFO_EMPTY) {
            // Get a payload from the transceiver
            pipe = nRF24_ReadPayload(nRF24_payload, &payload_length);

            // Clear all pending IRQ flags
            nRF24_ClearIRQFlags();

            // Print a payload contents to UART
//            UART_SendStr("RCV PIPE#");
//            UART_SendInt(pipe);
//            UART_SendStr(" PAYLOAD:>");
//            UART_SendBufHex((char *)nRF24_payload, payload_length);
//            UART_SendStr("<\r\n");
        }
    }
}


void nRF24_DEMO_RX_MULTI(){
    // This is simple receiver with multiple RX pipes:
    //   - pipe#0 address: "WBC"
    //   - pipe#0 payload: 11 bytes
    //   - pipe#1 address: '0xE7 0x1C 0xE3'
    //   - pipe#1 payload: 5 bytes
    //   - pipe#4 address: '0xE7 0x1C 0xE6' (this is pipe#1 address with different last byte)
    //   - pipe#4 payload: 32 bytes (the maximum payload length)
    //   - RF channel: 115 (2515MHz)
    //   - data rate: 250kbps (minimum possible, to increase reception reliability)
    //   - CRC scheme: 2 byte

    // The transmitter sends packets of different length to the three different logical addresses,
    // cycling them one after another, that packets comes to different pipes (0, 1 and 4)

    // Disable ShockBurst for all RX pipes
    nRF24_DisableAA(0xFF);

    // Set RF channel
    nRF24_SetRFChannel(115);

    // Set data rate
    nRF24_SetDataRate(nRF24_DR_250kbps);

    // Set CRC scheme
    nRF24_SetCRCScheme(nRF24_CRC_2byte);

    // Set address width, its common for all pipes (RX and TX)
    nRF24_SetAddrWidth(3);

    // Configure RX PIPE#0
    static const uint8_t nRF24_ADDR0[] = { 'W', 'B', 'C' };
    nRF24_SetAddr(nRF24_PIPE0, nRF24_ADDR0); // program address for RX pipe #0
    nRF24_SetRXPipe(nRF24_PIPE0, nRF24_AA_OFF, 11); // Auto-ACK: disabled, payload length: 11 bytes

    // Configure RX PIPE#1
    static const uint8_t nRF24_ADDR1[] = { 0xE7, 0x1C, 0xE3 };
    nRF24_SetAddr(nRF24_PIPE1, nRF24_ADDR1); // program address for RX pipe #1
    nRF24_SetRXPipe(nRF24_PIPE1, nRF24_AA_OFF, 5); // Auto-ACK: disabled, payload length: 5 bytes

    // Configure RX PIPE#4
    static const uint8_t nRF24_ADDR4[] = { 0xE6 };
    nRF24_SetAddr(nRF24_PIPE4, nRF24_ADDR4); // program address for RX pipe #4
    nRF24_SetRXPipe(nRF24_PIPE4, nRF24_AA_OFF, 32); // Auto-ACK: disabled, payload length: 32 bytes

    // Set operational mode (PRX == receiver)
    nRF24_SetOperationalMode(nRF24_MODE_RX);

    // Wake the transceiver
    nRF24_SetPowerMode(nRF24_PWR_UP);

    // Put the transceiver to the RX mode
    nRF24_CE_H;


    // The main loop
    while (1) {
        //
        // Constantly poll the status of the RX FIFO and get a payload if FIFO is not empty
        //
        // This is far from best solution, but it's ok for testing purposes
        // More smart way is to use the IRQ pin :)
        //
        if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY) {
            // Get a payload from the transceiver
            pipe = nRF24_ReadPayload(nRF24_payload, &payload_length);

            // Clear all pending IRQ flags
            nRF24_ClearIRQFlags();

            // Print a payload contents to UART
//            UART_SendStr("RCV PIPE#");
//            UART_SendInt(pipe);
//            UART_SendStr(" PAYLOAD:>");
//            UART_SendBufHex((char *)nRF24_payload, payload_length);
//            UART_SendStr("<\r\n");
        }
    }
}


void nRF24_DEMO_RX_SOLAR(){
    // This part is to receive data packets from the old solar-powered temperature sensor

    // Set RF channel
    nRF24_SetRFChannel(110);

    // Set data rate
    nRF24_SetDataRate(nRF24_DR_1Mbps);

    // Set CRC scheme
    nRF24_SetCRCScheme(nRF24_CRC_2byte);

    // Set address width, its common for all pipes (RX and TX)
    nRF24_SetAddrWidth(5);

    // Configure RX PIPE#0
#define RX_PAYLOAD 18
    // That address written here in reverse order because the old device that
    // transmits data uses old nRF24 "library" with errors :)
    static const uint8_t nRF24_ADDR[] = { 'T', 'k', 'l', 'o', 'W' };
    nRF24_SetAddr(nRF24_PIPE0, nRF24_ADDR); // program pipe address
    nRF24_SetRXPipe(nRF24_PIPE0, nRF24_AA_ON, RX_PAYLOAD); // Auto-ACK: enabled, payload length: 18 bytes

    // Configure TX PIPE address, this must be done for Auto-ACK (a.k.a. ShockBurst)
    // The address of TX PIPE must be same as it configured on the transmitter side
    nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR);

    // Set TX power to maximum (for more reliable Auto-ACK)
    nRF24_SetTXPower(nRF24_TXPWR_0dBm);

    // Set operational mode (PRX == receiver)
    nRF24_SetOperationalMode(nRF24_MODE_RX);

    // Wake the transceiver
    nRF24_SetPowerMode(nRF24_PWR_UP);

    // Put the transceiver to the RX mode
    nRF24_CE_H;


    int16_t temp;

    // The main loop
    while (1) {
        if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY) {
            // Get a payload from the transceiver
            pipe = nRF24_ReadPayload(nRF24_payload, &payload_length);

            // Clear all pending IRQ flags
            nRF24_ClearIRQFlags();

            // Print a payload contents to UART
//            UART_SendStr("RX=["); UART_SendBufHex((char *)nRF24_payload, payload_length);
//            UART_SendStr("] CRC=(");
//            UART_SendHex8(nRF24_payload[payload_length - 1]);
//            UART_SendStr(")\r\nTemperature: ");
            temp = (nRF24_payload[0] << 8) | nRF24_payload[1];
            if (temp < 0) {
                temp *= -1;
                //UART_SendChar('-');
            } else {
                //UART_SendChar('+');
            }
            //UART_SendInt(temp / 10); UART_SendChar('.');
            temp %= 10;
            //UART_SendInt(temp % 10); UART_SendStr("C\r\n");

//            UART_SendStr("Packet: #");
//            UART_SendInt((uint32_t)((nRF24_payload[2] << 24) | (nRF24_payload[3] << 16) | (nRF24_payload[4] << 8) | (nRF24_payload[5])));
//            UART_SendStr("\r\n");

            //uint8_t vrefint = (nRF24_payload[6] << 8) + nRF24_payload[7];
//            UART_SendStr("Vcc: ");
//            UART_SendInt(vrefint / 100);
//            UART_SendChar('.');
//            UART_SendInt0(vrefint % 100);
//            UART_SendStr("V\r\n");

            //uint8_t LSI_freq = (nRF24_payload[14] << 8) + nRF24_payload[15];
//            UART_SendStr("LSI: ");
//            UART_SendInt(LSI_freq);
//            UART_SendStr("Hz\r\nOBSERVE_TX:\r\n\t");
//            UART_SendHex8(nRF24_payload[16] >> 4);
//            UART_SendStr(" pckts lost\r\n\t");
//            UART_SendHex8(nRF24_payload[16] & 0x0F);
//            UART_SendStr(" retries\r\n");

//            uint8_t TR1 = nRF24_payload[8];
//            uint8_t TR2 = nRF24_payload[9];
//            uint8_t TR3 = nRF24_payload[10];
//            uint8_t DR1 = nRF24_payload[11];
//            uint8_t DR2 = nRF24_payload[12];
//            uint8_t DR3 = nRF24_payload[13];
//            uint8_t seconds = ((TR1 >> 4) * 10) + (TR1 & 0x0F);
//            uint8_t minutes = ((TR2 >> 4) * 10) + (TR2 & 0x0F);
//            uint8_t hours   = (((TR3 & 0x30) >> 4) * 10) + (TR3 & 0x0F);
//            uint8_t day   = ((DR1 >> 4) * 10) + (DR1 & 0x0F);
//            //dow   = DR2 >> 5;
//            uint8_t month = (((DR2 & 0x1F) >> 4) * 10) + (DR2 & 0x0F);
//            uint8_t year  = ((DR3 >> 4) * 10) + (DR3 & 0x0F);

//            UART_SendStr("Uptime: ");
//            UART_SendInt0(hours);
//            UART_SendChar(':');
//            UART_SendInt0(minutes);
//            UART_SendChar(':');
//            UART_SendInt0(seconds);
//            UART_SendChar(' ');
//            UART_SendInt0(day);
//            UART_SendChar('.');
//            UART_SendInt0(month);
//            UART_SendStr(".20");
//            UART_SendInt0(year);
//            UART_SendStr("\r\n");
        }
    }
}


void nRF24_DEMO_TX_SINGLE(){
    // This is simple transmitter (to one logic address):
    //   - TX address: '0xE7 0x1C 0xE3'
    //   - payload: 5 bytes
    //   - RF channel: 115 (2515MHz)
    //   - data rate: 250kbps (minimum possible, to increase reception reliability)
    //   - CRC scheme: 2 byte

    // The transmitter sends a 5-byte packets to the address '0xE7 0x1C 0xE3' without Auto-ACK (ShockBurst disabled)

    // Disable ShockBurst for all RX pipes
    nRF24_DisableAA(0xFF);

    // Set RF channel
    nRF24_SetRFChannel(115);

    // Set data rate
    nRF24_SetDataRate(nRF24_DR_250kbps);

    // Set CRC scheme
    nRF24_SetCRCScheme(nRF24_CRC_2byte);

    // Set address width, its common for all pipes (RX and TX)
    nRF24_SetAddrWidth(3);

    // Configure TX PIPE
    static const uint8_t nRF24_ADDR[] = { 0xE7, 0x1C, 0xE3 };
    nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR); // program TX address

    // Set TX power (maximum)
    nRF24_SetTXPower(nRF24_TXPWR_0dBm);

    // Set operational mode (PTX == transmitter)
    nRF24_SetOperationalMode(nRF24_MODE_TX);

    // Clear any pending IRQ flags
    nRF24_ClearIRQFlags();

    // Wake the transceiver
    nRF24_SetPowerMode(nRF24_PWR_UP);


    // The main loop
    j = 0;
    payload_length = 5;
    while (1) {
        // Prepare data packet
        for (i = 0; i < payload_length; i++) {
            nRF24_payload[i] = j++;
            if (j > 0x000000FF) j = 0;
        }

        // Print a payload
//        UART_SendStr("PAYLOAD:>");
//        UART_SendBufHex((char *)nRF24_payload, payload_length);
//        UART_SendStr("< ... TX: ");

        // Transmit a packet
        nRF24_TXResult tx_res = nRF24_TransmitPacket(nRF24_payload, payload_length);
        switch (tx_res) {
        case nRF24_TX_SUCCESS:
            //UART_SendStr("OK");
            break;
        case nRF24_TX_TIMEOUT:
            //UART_SendStr("TIMEOUT");
            break;
        case nRF24_TX_MAXRT:
            //UART_SendStr("MAX RETRANSMIT");
            break;
        default:
            //UART_SendStr("ERROR");
            break;
        }
        //UART_SendStr("\r\n");

        // Wait ~0.5s
        Rudiron::CLK::delay_millis(500);
    }
}


void nRF24_DEMO_TX_MULTI(){
    // This is simple transmitter (to multiple logic addresses):
    //   - TX addresses and payload lengths:
    //       'WBC', 11 bytes
    //       '0xE7 0x1C 0xE3', 5 bytes
    //       '0xE7 0x1C 0xE6', 32 bytes
    //   - RF channel: 115 (2515MHz)
    //   - data rate: 250kbps (minimum possible, to increase reception reliability)
    //   - CRC scheme: 2 byte

    // The transmitter sends a data packets to the three logic addresses without Auto-ACK (ShockBurst disabled)
    // The payload length depends on the logic address

    // Disable ShockBurst for all RX pipes
    nRF24_DisableAA(0xFF);

    // Set RF channel
    nRF24_SetRFChannel(115);

    // Set data rate
    nRF24_SetDataRate(nRF24_DR_250kbps);

    // Set CRC scheme
    nRF24_SetCRCScheme(nRF24_CRC_2byte);

    // Set address width, its common for all pipes (RX and TX)
    nRF24_SetAddrWidth(3);

    // Set TX power (maximum)
    nRF24_SetTXPower(nRF24_TXPWR_0dBm);

    // Set operational mode (PTX == transmitter)
    nRF24_SetOperationalMode(nRF24_MODE_TX);

    // Clear any pending IRQ flags
    nRF24_ClearIRQFlags();

    // Wake the transceiver
    nRF24_SetPowerMode(nRF24_PWR_UP);

    static const uint8_t nRF24_ADDR0[] = { 'W', 'B', 'C' };
    static const uint8_t nRF24_ADDR1[] = { 0xE7, 0x1C, 0xE3 };
    static const uint8_t nRF24_ADDR2[] = { 0xE7, 0x1C, 0xE6 };

    // The main loop
    j = 0; pipe = (nRF24_RXResult)0;
    while (1) {
        // Logic address
//        UART_SendStr("ADDR#");
//        UART_SendInt(pipe);

        // Configure the TX address and payload length
        switch (pipe) {
        case 0:
            // addr #1
            nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR0);
            payload_length = 11;
            break;
        case 1:
            // addr #2
            nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR1);
            payload_length = 5;
            break;
        case 2:
            // addr #3
            nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR2);
            payload_length = 32;
            break;
        default:
            break;
        }

        // Prepare data packet
        for (i = 0; i < payload_length; i++) {
            nRF24_payload[i] = j++;
            if (j > 0x000000FF) j = 0;
        }

        // Print a payload
//        UART_SendStr(" PAYLOAD:>");
//        UART_SendBufHex((char *)nRF24_payload, payload_length);
//        UART_SendStr("< ... TX: ");

        // Transmit a packet
        nRF24_TXResult tx_res = nRF24_TransmitPacket(nRF24_payload, payload_length);
        switch (tx_res) {
        case nRF24_TX_SUCCESS:
            //UART_SendStr("OK");
            break;
        case nRF24_TX_TIMEOUT:
            //UART_SendStr("TIMEOUT");
            break;
        case nRF24_TX_MAXRT:
            //UART_SendStr("MAX RETRANSMIT");
            break;
        default:
            //UART_SendStr("ERROR");
            break;
        }
        //UART_SendStr("\r\n");

        // Proceed to next address
        pipe = (nRF24_RXResult)((uint8_t)pipe + 1);
        if (pipe > 2) {
            pipe = (nRF24_RXResult)0;
        }

        // Wait ~0.5s
        Rudiron::CLK::delay_millis(500);
    }
}
