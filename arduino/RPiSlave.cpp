#include "Arduino.h"
#include "RpiSlave.h"
#include <util/twi.h>

char data[256] = "Hello, world!";

unsigned char RPiSlave::getByte(unsigned char index)
{
  return data[index];
}

void RPiSlave::setByte(unsigned char index, unsigned char value)
{
  data[index] = value;
}

unsigned char RPiSlave::checkForCommand()
{
  if(CMD_STATUS_CALL == data[CMD_STATUS])
    return data[1];
  return 0;
}

void RPiSlave::commandReturn()
{
  data[CMD_STATUS] = CMD_STATUS_RETURN;
}
  
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

uint8_t RPiSlave::handle_event(unsigned char event, unsigned char write_data)
{
  static int index;
  static int index_set = 0;
  
  switch(event)
  {
    case TW_SR_SLA_ACK:
      delayMicroseconds(10);
      index = 0; // go to the beginning of the array
      index_set = 0;
      break;
    case TW_SR_DATA_ACK:
      delayMicroseconds(10);
      if(!index_set)
      {
        index = write_data;
        index_set = 1;
      }
      else
      {
        if(index < 128)
          data[CMD_STATUS] = CMD_STATUS_LOCK;
        data[index] = write_data;
        index ++;
      }
      break;
    case TW_SR_DATA_NACK:
      return 1;
    case TW_ST_SLA_ACK:
    case TW_ST_DATA_ACK:
      TWDR = data[index];
      index ++;
      break;
    case TW_SR_STOP: // master is done sending
      if(CMD_STATUS_LOCK == data[0])
        data[CMD_STATUS] = CMD_STATUS_CALL;
      break;
  }
  
  return 0; // default - ACK
}
