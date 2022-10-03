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
*/

extern "C"
{
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
  // #include "utility/twi.h"
}

#include "MDR_i2c.h"

#include "Wire.h"

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
}

// Public Methods //////////////////////////////////////////////////////////////

//Начало обмена данными через обработчики прерываний
void TwoWire::begin(void)
{
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;

  setClock(400000);

  I2C_TX_Event = &onRequestService;
  I2C_RX_Event = &onReceiveService;

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

//Конец обмена данными через обработчики прерываний
void TwoWire::end(void)
{
  I2C_Cmd(DISABLE);
}

void TwoWire::setClock(uint32_t clock)
{
  I2C_InitTypeDef initStruct;

  if (clock <= 400000){
    initStruct.I2C_Speed = I2C_SPEED_UP_TO_400KHz;
  }
  else {
    initStruct.I2C_Speed = I2C_SPEED_UP_TO_1MHz;
  }
  
  initStruct.I2C_Speed = (32 / (clock / 100000)) - 1;
  I2C_Init(&initStruct);
}

/***
 * Sets the TWI timeout.
 *
 * This limits the maximum time to wait for the TWI hardware. If more time passes, the bus is assumed
 * to have locked up (e.g. due to noise-induced glitches or faulty slaves) and the transaction is aborted.
 * Optionally, the TWI hardware is also reset, which can be required to allow subsequent transactions to
 * succeed in some cases (in particular when noise has made the TWI hardware think there is a second
 * master that has claimed the bus).
 *
 * When a timeout is triggered, a flag is set that can be queried with `getWireTimeoutFlag()` and is cleared
 * when `clearWireTimeoutFlag()` or `setWireTimeoutUs()` is called.
 *
 * Note that this timeout can also trigger while waiting for clock stretching or waiting for a second master
 * to complete its transaction. So make sure to adapt the timeout to accomodate for those cases if needed.
 * A typical timeout would be 25ms (which is the maximum clock stretching allowed by the SMBus protocol),
 * but (much) shorter values will usually also work.
 *
 * In the future, a timeout will be enabled by default, so if you require the timeout to be disabled, it is
 * recommended you disable it by default using `setWireTimeoutUs(0)`, even though that is currently
 * the default.
 *
 * @param timeout a timeout value in microseconds, if zero then timeout checking is disabled
 * @param reset_with_timeout if true then TWI interface will be automatically reset on timeout
 *                           if false then TWI interface will not be reset on timeout

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

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
  if (isize > 0)
  {
    // send internal address; this mode allows sending a repeated start to access
    // some devices' internal registers. This function is executed by the hardware
    // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

    beginTransmission(address);

    // the maximum size of internal address is 3 bytes
    if (isize > 3)
    {
      isize = 3;
    }

    // write internal register address - most significant byte first
    while (isize-- > 0)
      write((uint8_t)(iaddress >> (isize * 8)));
    endTransmission(false);
  }

  // clamp to buffer length
  if (quantity > BUFFER_LENGTH)
  {
    quantity = BUFFER_LENGTH;
  }
  // perform blocking read into buffer
  uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read;

  return read;
}

uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
  uint8_t i;

  // // ensure data will fit into buffer
  // if(TWI_BUFFER_LENGTH < length){
  //   return 0;
  // }

  // // wait until twi is ready, become master receiver
  // uint32_t startMicros = micros();
  // while(TWI_READY != twi_state){
  //   if((twi_timeout_us > 0ul) && ((micros() - startMicros) > twi_timeout_us)) {
  //     twi_handleTimeout(twi_do_reset_on_timeout);
  //     return 0;
  //   }
  // }
  // twi_state = TWI_MRX;
  // twi_sendStop = sendStop;
  // // reset error state (0xFF.. no error occured)
  // twi_error = 0xFF;

  // // initialize buffer iteration vars
  // twi_masterBufferIndex = 0;
  // twi_masterBufferLength = length-1;  // This is not intuitive, read on...
  // // On receive, the previously configured ACK/NACK setting is transmitted in
  // // response to the received byte before the interrupt is signalled. 
  // // Therefor we must actually set NACK when the _next_ to last byte is
  // // received, causing that NACK to be sent in response to receiving the last
  // // expected byte of data.

  // // build sla+w, slave device address + w bit
  // twi_slarw = TW_READ;
  // twi_slarw |= address << 1;

  // if (true == twi_inRepStart) {
  //   // if we're in the repeated start state, then we've already sent the start,
  //   // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
  //   // We need to remove ourselves from the repeated start state before we enable interrupts,
  //   // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
  //   // up. Also, don't enable the START interrupt. There may be one pending from the 
  //   // repeated start that we sent ourselves, and that would really confuse things.
  //   twi_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
  //   startMicros = micros();
  //   do {
  //     TWDR = twi_slarw;
  //     if((twi_timeout_us > 0ul) && ((micros() - startMicros) > twi_timeout_us)) {
  //       twi_handleTimeout(twi_do_reset_on_timeout);
  //       return 0;
  //     }
  //   } while(TWCR & _BV(TWWC));
  //   TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);	// enable INTs, but not START
  // } else {
  //   // send start condition
  //   TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
  // }

  // // wait for read operation to complete
  // startMicros = micros();
  // while(TWI_MRX == twi_state){
  //   if((twi_timeout_us > 0ul) && ((micros() - startMicros) > twi_timeout_us)) {
  //     twi_handleTimeout(twi_do_reset_on_timeout);
  //     return 0;
  //   }
  // }

  // if (twi_masterBufferIndex < length) {
  //   length = twi_masterBufferIndex;
  // }

  // // copy twi buffer to data
  // for(i = 0; i < length; ++i){
  //   data[i] = twi_masterBuffer[i];
  // }

  // return length;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
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

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to
//	perform a repeated start.
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
  // transmit buffer (blocking)
  uint8_t ret = writeTo(txAddress, txBuffer, txBufferLength, 1, sendStop);
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  // indicate that we are done transmitting
  transmitting = 0;
  return ret;
}

uint8_t writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
  uint8_t i;

  // // ensure data will fit into buffer
  // if(TWI_BUFFER_LENGTH < length){
  //   return 1;
  // }

  // // wait until twi is ready, become master transmitter
  // uint32_t startMicros = micros();
  // while(TWI_READY != twi_state){
  //   if((twi_timeout_us > 0ul) && ((micros() - startMicros) > twi_timeout_us)) {
  //     twi_handleTimeout(twi_do_reset_on_timeout);
  //     return (5);
  //   }
  // }
  // twi_state = TWI_MTX;
  // twi_sendStop = sendStop;
  // // reset error state (0xFF.. no error occured)
  // twi_error = 0xFF;

  // // initialize buffer iteration vars
  // twi_masterBufferIndex = 0;
  // twi_masterBufferLength = length;
  
  // // copy data to twi buffer
  // for(i = 0; i < length; ++i){
  //   twi_masterBuffer[i] = data[i];
  // }
  
  // // build sla+w, slave device address + w bit
  // twi_slarw = TW_WRITE;
  // twi_slarw |= address << 1;
  
  // // if we're in a repeated start, then we've already sent the START
  // // in the ISR. Don't do it again.
  // //
  // if (true == twi_inRepStart) {
  //   // if we're in the repeated start state, then we've already sent the start,
  //   // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
  //   // We need to remove ourselves from the repeated start state before we enable interrupts,
  //   // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
  //   // up. Also, don't enable the START interrupt. There may be one pending from the 
  //   // repeated start that we sent outselves, and that would really confuse things.
  //   twi_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
  //   startMicros = micros();
  //   do {
  //     TWDR = twi_slarw;
  //     if((twi_timeout_us > 0ul) && ((micros() - startMicros) > twi_timeout_us)) {
  //       twi_handleTimeout(twi_do_reset_on_timeout);
  //       return (5);
  //     }
  //   } while(TWCR & _BV(TWWC));
  //   TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);	// enable INTs, but not START
  // } else {
  //   // send start condition
  //   TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA);	// enable INTs
  // }

  // // wait for write operation to complete
  // startMicros = micros();
  // while(wait && (TWI_MTX == twi_state)){
  //   if((twi_timeout_us > 0ul) && ((micros() - startMicros) > twi_timeout_us)) {
  //     twi_handleTimeout(twi_do_reset_on_timeout);
  //     return (5);
  //   }
  // }
  
  // if (twi_error == 0xFF)
  //   return 0;	// success
  // else if (twi_error == TW_MT_SLA_NACK)
  //   return 2;	// error: address send, nack received
  // else if (twi_error == TW_MT_DATA_NACK)
  //   return 3;	// error: data send, nack received
  // else
  //   return 4;	// other twi error
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
  else
  {
    // in slave send mode
    // reply to master
    twi_transmit(&data, 1);
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
  else
  {
    // in slave send mode
    // reply to master
    twi_transmit(data, quantity);
  }
  return quantity;
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

void TwoWire::flush(void)
{
  // XXX: to be implemented.
}

//Обработчик прерывания на прием данных
// behind the scenes function that is called when data is received
void TwoWire::onReceiveService(uint8_t *inBytes, int numBytes)
{
  // don't bother if user hasn't registered a callback
  if (!user_onReceive)
  {
    return;
  }
  // don't bother if rx buffer is in use by a master requestFrom() op
  // i know this drops data, but it allows for slight stupidity
  // meaning, they may not have read all the master requestFrom() data yet
  if (rxBufferIndex < rxBufferLength)
  {
    return;
  }
  // copy twi rx buffer into local read buffer
  // this enables new reads to happen in parallel
  for (uint8_t i = 0; i < numBytes; ++i)
  {
    rxBuffer[i] = inBytes[i];
  }
  // set rx iterator vars
  rxBufferIndex = 0;
  rxBufferLength = numBytes;
  // alert user program
  user_onReceive(numBytes);
}

//Обработчик прерывания на отправку данных
// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
  // don't bother if user hasn't registered a callback
  if (!user_onRequest)
  {
    return;
  }
  // reset tx buffer iterator vars
  // !!! this will kill any pending pre-master sendTo() activity
  txBufferIndex = 0;
  txBufferLength = 0;
  // alert user program
  user_onRequest();
}

// sets function called on slave write
void TwoWire::onReceive(void (*function)(int))
{
  user_onReceive = function;
}

// sets function called on slave read
void TwoWire::onRequest(void (*function)(void))
{
  user_onRequest = function;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();
