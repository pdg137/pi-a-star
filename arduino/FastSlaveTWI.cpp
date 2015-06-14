#include "Arduino.h"
#include "FastSlaveTWI.h"
#include <util/twi.h>

FastSlaveTWI::Slave *slave;

void FastSlaveTWI::init(unsigned char address, Slave &my_slave)
{
  slave = &my_slave;
  TWAR = address << 1;
  digitalWrite(SDA, 1);
  digitalWrite(SCL, 1);
  ack();
}

ISR(TWI_vect)
{
  if(FastSlaveTWI::handle_event(TWSR))
    FastSlaveTWI::nack();
  else
    FastSlaveTWI::ack();
}

void FastSlaveTWI::ack()
{
  TWCR = 
    (1<<TWEN)    // enable TWI
    | (1<<TWIE)  // enable TWI interrupt
    | (1<<TWINT) // clear interrupt flag
    | (1<<TWEA); // ACK
}

void FastSlaveTWI::nack()
{
  TWCR = 
    (1<<TWEN)     // enable TWI
    | (1<<TWIE)   // enable TWI interrupt
    | (1<<TWINT); // clear interrupt flag
}

uint8_t FastSlaveTWI::handle_event(unsigned char event)
{
  // See the ATmega32U4 datasheet for a list of I2C states and responses.
  // We are ignoring any master-related states and general calls.
  switch(event)
  {
    /*** Slave receiver mode ***/
    case TW_SR_SLA_ACK: // addressed and ACKed -> ACK next data byte
      slave->start();
      break;
    case TW_SR_DATA_ACK: // received a data byte and ACKed -> ACK next byte
      slave->receive(TWDR);
      break;
    case TW_SR_DATA_NACK: // received data, NACKed - should never happen - go back to ACK
      break;
    case TW_SR_STOP: // A STOP or repeated START - nothing to do here but ACK
      slave->stop();
      break;
      
    /*** Slave transmitter mode ***/
    case TW_ST_SLA_ACK: // addressed and ACKed
    case TW_ST_DATA_ACK: // transmitted a byte and got ACK -> transmit the next byte and ACK 
      TWDR = slave->transmit();
      break;
    case TW_ST_DATA_NACK: // transmitted a byte and got NACK -> done sending
      break;
    case TW_ST_LAST_DATA: // transmitted a byte with TWEA=0 - shouldn't happen
      break;

    /*** Misc states ***/
    case TW_NO_INFO: // not sure how this can happen - TWINT=0 according to datasheet
      // ideally we would not change TWCR here, but our code will ACK
      break;
    case TW_BUS_ERROR: // error on the bus - set TWSTO and TWINT and ACK
      TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT) | (1<<TWEA);
      // the hardware will now clear TWSTO automatically
      break; 
  }
  
  return 0; // default - ACK
}
