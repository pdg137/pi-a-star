#include "Arduino.h"
#include "RpiSlave.h"
#include <util/twi.h>

void RPiSlave::init(unsigned char address)
{
  TWAR = address << 1;
  ack();
}

void leds(char y, char g, char r)
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
  if(RPiSlave::handle_event(TWSR, TWDR))
    RPiSlave::nack();
  else
    RPiSlave::ack();
}

void RPiSlave::ack()
{
  TWCR = 
    (1<<TWEN)    // enable TWI
    | (1<<TWIE)  // enable TWI interrupt
    | (1<<TWINT) // clear interrupt flag
    | (1<<TWEA); // ACK
}

void RPiSlave::nack()
{
  TWCR = 
    (1<<TWEN)     // enable TWI
    | (1<<TWIE)   // enable TWI interrupt
    | (1<<TWINT); // clear interrupt flag
}

uint8_t RPiSlave::handle_event(unsigned char event, unsigned char data)
{
  switch(event)
  {
    case TW_SR_SLA_ACK:
      delayMicroseconds(10);
      return 0;
    case TW_SR_DATA_ACK:
      delayMicroseconds(10);
      leds((data & 1) != 0, (data & 2) != 0, (data & 4) != 0);
      return 0;
    case TW_SR_DATA_NACK:
      return 1;
    case TW_ST_DATA_ACK:
      TWDR = 0; // put data here
  }
  
  return 0; // default - ACK
}
