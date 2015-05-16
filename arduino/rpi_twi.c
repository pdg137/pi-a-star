/*
  twi.c - TWI/I2C library for Wiring & Arduino
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
*/

#include <compat/twi.h>
#include "Arduino.h" // for digitalWrite

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "pins_arduino.h"
#include "rpi_twi.h"

static void (*twi_onSlaveTransmit)(void);
static void (*twi_onSlaveReceive)(uint8_t*, int);

/* 
 * Function twi_init
 * Desc     readys twi pins and sets twi bitrate
 * Input    none
 * Output   none
 */
void twi_init(void)
{
  // activate internal pullups for twi.
  digitalWrite(SDA, 1);
  digitalWrite(SCL, 1);

  // initialize twi prescaler and bit rate
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  
  // enable twi module, acks, and twi interrupt
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

/* 
 * Function twi_slaveInit
 * Desc     sets slave address and enables interrupt
 * Input    none
 * Output   none
 */
void twi_setAddress(uint8_t address)
{
  // set twi slave address (skip over TWGCE bit)
  TWAR = address << 1;
}

/* 
 * Function twi_reply
 * Desc     sends byte or readys receive line
 * Input    ack: byte indicating to ack or to nack
 * Output   none
 */
void ack()
{
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
}

void nack()
{
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
}


void leds(uint8_t y, uint8_t g, uint8_t r)
{
  digitalWrite(13, y);
  if(g)
    TXLED1;
  else
    TXLED0;
    
  if(r)
    RXLED1;
  else
    RXLED0;
}

ISR(TWI_vect)
{
  switch(TW_STATUS){
    case TW_SR_SLA_ACK:   // addressed, returned ack
      delayMicroseconds(10);
    case TW_SR_GCALL_ACK: // addressed generally, returned ack
    case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
    case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
      // indicate that rx buffer can be overwritten and ack
      ack();
      break;
    case TW_SR_DATA_ACK:       // data received, returned ack
      delayMicroseconds(10);
    case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
      leds((TWDR & 1) != 0, (TWDR & 2) != 0, (TWDR & 4) != 0);
      // if there is still room in the rx buffer
      // put byte in buffer and ack
      ack(); // or nack() if no room
      break;
    case TW_SR_STOP: // stop or repeated start condition received      
      // ack future responses and leave slave receiver state
      ack();
      
      // this is where a callback would go
      
      break;
    case TW_SR_DATA_NACK:       // data received, returned nack
    case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
      // nack back at master
      nack();
      break;
    
    // Slave Transmitter
    case TW_ST_SLA_ACK:          // addressed, returned ack
    case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
      // request for txBuffer to be filled and length to be set
      // note: user must call twi_transmit(bytes, length) to do this
      // get buffer reader for transmission here
      // transmit first byte from buffer, fall through
    case TW_ST_DATA_ACK: // byte sent, ack returned
      // copy data to output register
      TWDR = 0; // <-- put data here
      // if there is more to send, ack, otherwise nack
      ack();
      break;
    case TW_ST_DATA_NACK: // received nack, we are done 
    case TW_ST_LAST_DATA: // received ack, but we are done already!
      // ack future responses
      ack();
      break;

    // All
    case TW_NO_INFO:   // no state information
      break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
      break;
  }
}

