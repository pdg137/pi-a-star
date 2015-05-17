#include "Arduino.h"
#include "RpiSlave.h"
#include <util/twi.h>

char data[256] = "Hello, world!";
unsigned char index;
unsigned char index_set = 0;

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
  // When the CMD_STATUS byte is set to CMD_STATUS_CALL, it means
  // that a command has been issued by the Raspberry Pi.  The command
  // number is stored in the CMD_NUMBER byte.  Zero, the default,
  // means no command.
  if(CMD_STATUS_CALL == data[CMD_STATUS])
    return data[CMD_NUMBER];
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
  if(RPiSlave::handle_event(TWSR))
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

void reset_index()
{
  index = 0; // go to the beginning of the array
  index_set = 0;
}

// delay to accomodate the Broadcom I2C bug.
void rpi_delay()
{
  delayMicroseconds(10);
}

// reads the available byte and stores it in index
void read_index()
{
  index = TWDR;
  index_set = 1;
}

// Write the lock value to status if we are about to write a command.
void lock_if_writing_command()
{
  if(index < 128)
    data[CMD_STATUS] = CMD_STATUS_LOCK;
}

// Write a byte and increment the index.
void write_and_increment()
{
  data[index] = TWDR;
  index ++;
}

void read_and_increment()
{
  TWDR = data[index];
  index ++;
}

// Sets the status to "call" if it is in "lock".
void call_if_locked_for_command()
{
  if(CMD_STATUS_LOCK == data[0])
    data[CMD_STATUS] = CMD_STATUS_CALL;
}

uint8_t RPiSlave::handle_event(unsigned char event)
{
  
  switch(event)
  {
    case TW_SR_SLA_ACK:
      rpi_delay();
      reset_index();
      break;
    case TW_SR_DATA_ACK:
      rpi_delay();
      if(!index_set)
      {
        read_index();
      }
      else
      {
        lock_if_writing_command();
        write_and_increment();
      }
      break;
    case TW_SR_DATA_NACK:
      return 1;
    case TW_ST_SLA_ACK:
    case TW_ST_DATA_ACK:
      read_and_increment();
      break;
    case TW_SR_STOP: // master is done sending
      call_if_locked_for_command();
      break;
  }
  
  return 0; // default - ACK
}
