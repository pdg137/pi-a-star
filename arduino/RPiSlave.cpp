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
  digitalWrite(SDA, 1);
  digitalWrite(SCL, 1);
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
  // See the ATmega32U4 datasheet for a list of I2C states and responses.
  // We are ignoring any master-related states and general calls.
  switch(event)
  {
    /*** Slave receiver mode ***/
    case TW_SR_SLA_ACK: // addressed and ACKed -> ACK next data byte
      rpi_delay();
      reset_index();
      break;
    case TW_SR_DATA_ACK: // received a data byte and ACKed -> ACK next byte
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
    case TW_SR_DATA_NACK: // received data, NACKed - should never happen - go back to ACK
      break;
    case TW_SR_STOP: // A STOP or repeated START - nothing to do here but ACK
      call_if_locked_for_command();
      break;
      
    /*** Slave transmitter mode ***/
    case TW_ST_SLA_ACK: // addressed and ACKed
    case TW_ST_DATA_ACK: // transmitted a byte and got ACK -> transmit the next byte and ACK 
      read_and_increment();
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
      TWCR = (1<<TWSTO) | (1<<TWINT) | (1<<TWEA);
      RXLED1;
      // the hardware will now clear TWSTO automatically
      break; 
  }
  
  return 0; // default - ACK
}
