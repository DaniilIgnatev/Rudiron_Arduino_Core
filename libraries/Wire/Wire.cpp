/*
  TwoWire.cpp - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2017 by Chuck Todd (ctodd@cableone.net) to correct Unconfigured Slave Mode reboot
  Modified 2020 by Greyson Christoforo (grey@christoforo.net) to implement timeouts
   Modified 2022 by Daniil Ignatev for Arduino_Core_Rudiron_MDR32F9Qx
*/

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
// #include "utility/twi.h"

#include "rudiron/gpio.h"
#include "MDR32F9Qx_i2c.h"
#include "Wire.h"

using namespace Rudiron;

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
uint8_t TwoWire::txAddress = 0;

uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

void configGPIO(bool SDA_output)
{
    PORT_InitTypeDef PORT_InitStructure;
    PORT_StructInit(&PORT_InitStructure);

    PORT_InitStructure.PORT_PD = PORT_PD_OPEN;
    PORT_InitStructure.PORT_FUNC = PORT_FUNC_ALTER;
    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;

    // SDA
    PORT_InitStructure.PORT_OE = (PORT_OE_TypeDef)SDA_output;
    GPIO::configPin(PORT_PIN_C1, PORT_InitStructure);

    // SCL
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    GPIO::configPin(PORT_PIN_C0, PORT_InitStructure);
}

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
}

// Public Methods //////////////////////////////////////////////////////////////

// Начало обмена данными через обработчики прерываний
void TwoWire::begin(void)
{
    rxBufferIndex = 0;
    rxBufferLength = 0;

    txBufferIndex = 0;
    txBufferLength = 0;

    configGPIO(true);

    RST_CLK_PCLKcmd(RST_CLK_PCLK_I2C, ENABLE);
    setClock(100 * 1000);

    I2C_ITConfig(ENABLE);
    I2C_Cmd(ENABLE);
}

void TwoWire::begin(uint8_t address)
{
    begin();
}

void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

// Конец обмена данными через обработчики прерываний
void TwoWire::end(void)
{
    I2C_Cmd(DISABLE);
    I2C_ITConfig(DISABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_I2C, DISABLE);
}

void TwoWire::setClock(uint32_t clock)
{
    I2C_InitTypeDef initStruct;

    if (clock < 400000)
    {
        initStruct.I2C_Speed = I2C_SPEED_UP_TO_400KHz;
    }
    else
    {
        initStruct.I2C_Speed = I2C_SPEED_UP_TO_1MHz;
    }

    // добавить адаптацию к частоте цпу!
    initStruct.I2C_ClkDiv = (32 / (clock / 100000)) - 1;
    if (!IS_I2C_CLKDIV(initStruct.I2C_ClkDiv))
    {
        return;
    }

    I2C_Init(&initStruct);
}

/*
 * This limits the maximum time to wait for the TWI hardware. If more time passes, the bus is assumed
 * to have locked up
 */
void TwoWire::setWireTimeout(uint32_t timeout, bool reset_with_timeout)
{
    // twi_setTimeoutInMicros(timeout, reset_with_timeout);
}

/***
 * Returns the TWI timeout flag.
 *
 * @return true if timeout has occured since the flag was last cleared.
 */
bool TwoWire::getWireTimeoutFlag(void)
{
    // return(twi_manageTimeoutFlag(false));
}

/***
 * Clears the TWI timeout flag.
 */
void TwoWire::clearWireTimeoutFlag(void)
{
    // twi_manageTimeoutFlag(true);
}

void TwoWire::beginTransmission(uint8_t address)
{
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address;
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

// Ожидание освобождения шины
bool check_BUS_FREE(int timeout_us)
{
    if (I2C_CheckEvent(I2C_EVENT_BUS_HELD) == SUCCESS ){
        return true;
    }

    uint32_t startMicros = micros();
    while (I2C_CheckEvent(I2C_EVENT_BUS_FREE) != SUCCESS)
    {
        if ((micros() - startMicros) > timeout_us)
        {
            return false;
        }
    }

    return true;
}

// Проверка занятия шины
bool check_TRANSFER_ENABLED(int timeout_us)
{
    uint32_t startMicros = micros();

    while (I2C_CheckEvent(I2C_EVENT_TRANSFER_ENABLED) != SUCCESS)
    {
        if (micros() - startMicros > timeout_us)
        {
            return false;
        }
    }

    return true;
}

// Проверка подтверждения
bool check_ACK_FOUND(int timeout_us)
{
    uint32_t startMicros = micros();

    while (true)
    {
        if (micros() - startMicros > timeout_us)
        {
            return false;
        }

        if (I2C_CheckEvent(I2C_EVENT_ACK_FOUND))
        {
            return true;
        }
    }
}

// Выгрузка буфера в шину
uint8_t TwoWire::writeSync(uint8_t address, uint8_t *data, uint8_t length, uint8_t sendStop)
{
    if (!transmitting || txBuffer == NULL)
    {
        return 4;
    }

    if (!check_BUS_FREE(10 * 1000))
    {
        return 5;
    }

    I2C_Send7bitAddress(address, I2C_Direction_Transmitter);
    if (!check_TRANSFER_ENABLED(10 * 1000))
    {
        return 5;
    }

    if (!check_ACK_FOUND(10 * 1000))
    {
        I2C_SendSTOP();
        return 2;
    }

    for (int i = 0; i < length; ++i)
    {
        I2C_SendByte(data[i]);
        if (!check_ACK_FOUND(10 * 1000))
        {
            I2C_SendSTOP();
            return 3;
        }
    }

    if (sendStop)
    {
        I2C_SendSTOP();
    }

    return 0;
}

/*
https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
endTransmission() returns:
0: success.
1: data too long to fit in transmit buffer.
2: received NACK on transmit of address.
3: received NACK on transmit of data.
4: other error.
5: timeout
*/
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    // transmit buffer (blocking)
    uint8_t ret = writeSync(txAddress << 1, txBuffer, txBufferLength, sendStop);
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
    // indicate that we are done transmitting
    transmitting = 0;
    return ret;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
    if (transmitting)
    {
        // in master transmitter mode
        // don't bother if buffer is full
        if (txBufferLength >= BUFFER_LENGTH)
        {
            setWriteError();
            return 0;
        }
        // put byte in tx buffer
        txBuffer[txBufferIndex] = data;
        ++txBufferIndex;
        // update amount in buffer
        txBufferLength = txBufferIndex;
    }

    return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    if (transmitting)
    {
        // in master transmitter mode
        for (size_t i = 0; i < quantity; ++i)
        {
            write(data[i]);
        }
    }

    return quantity;
}

void TwoWire::flush(void)
{
    // XXX: to be implemented.
}

// Проверка освобождения шины
bool check_TRANSFER_DISABLED(int timeout_us)
{
    uint32_t startMicros = micros();

    while (I2C_CheckEvent(I2C_EVENT_TRANSFER_ENABLED) == SUCCESS)
    {
        if (micros() - startMicros > timeout_us)
        {
            return false;
        }
    }

    return true;
}

uint8_t TwoWire::readSync(uint8_t address, uint8_t *data, uint8_t length, uint8_t sendStop)
{
    if (!check_BUS_FREE(10 * 1000))
    {
        return 0;
    }

    I2C_Send7bitAddress(address, I2C_Direction_Receiver);
    if (!check_TRANSFER_ENABLED(10 * 1000))
    {
        return 0;
    }

    if (!check_ACK_FOUND(10 * 1000))
    {
        I2C_SendSTOP();
        return 0;
    }

    for (int i = 0; i < length; ++i)
    {
        I2C_StartReceiveData(i < length - 1 ? I2C_Send_to_Slave_ACK : I2C_Send_to_Slave_NACK);

        if (!check_TRANSFER_ENABLED(10 * 1000))
        {
            I2C_SendSTOP();
            return i;
        }

        data[i] = I2C_GetReceivedData();
    }

    if (sendStop)
    {
        I2C_SendSTOP();
    }

    return length;
}

/// @brief Выполняет запрос данных
/// @param address Адрес второго устройства
/// @param quantity Сколько байт хочется получить
/// @param sendStop Отпустить шину после приема данных
/// @return Количество полученных байт
uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    // clamp to buffer length
    if (quantity > BUFFER_LENGTH)
    {
        quantity = BUFFER_LENGTH;
    }

    // perform blocking read into buffer
    uint8_t read = readSync(address << 1, rxBuffer, quantity, sendStop);

    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = read;

    return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
    return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
    int value = -1;

    // get each successive byte on each call
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }

    return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
    int value = -1;

    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
    }

    return value;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();
