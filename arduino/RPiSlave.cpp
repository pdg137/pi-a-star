#include "Arduino.h"
#include "RpiSlave.h"
#include "FastSlaveTWI.h"

char data[256] = "Hello, world!";
unsigned char index;
unsigned char index_set = 0;
RPiSlave::Slave rpi_slave_instance;

unsigned char RPiSlave::getByte(unsigned char index)
{
  return data[index];
}

short RPiSlave::getInt16(unsigned char index)
{
  return *(short *)&data[index];
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

void reset_index()
{
  index = 0; // go to the beginning of the array
  index_set = 0;
}

// Write the lock value to status if we are about to write a command.
void lock_if_writing_command()
{
  if(index < 128)
    data[CMD_STATUS] = CMD_STATUS_LOCK;
}

// Sets the status to "call" if it is in "lock".
void call_if_locked_for_command()
{
  if(CMD_STATUS_LOCK == data[0])
    data[CMD_STATUS] = CMD_STATUS_CALL;
}

// delay to accomodate the Broadcom I2C bug.
void RPiSlave::Slave::piDelay()
{
  delayMicroseconds(10);
}

void RPiSlave::Slave::receive(unsigned char b)
{
  piDelay();
  if(!index_set)
  {
    index = b;
    index_set = 1;
  }
  else
  {
    lock_if_writing_command();
    data[index] = b;
    index ++;
  }
}

void RPiSlave::Slave::start()
{
  piDelay();
  reset_index();
}

void RPiSlave::Slave::stop()
{
  call_if_locked_for_command();
}

unsigned char RPiSlave::Slave::transmit()
{
  piDelay();
  return data[index++];
}
  
void RPiSlave::init(unsigned char address)
{
  FastSlaveTWI::init(address, rpi_slave_instance);
}
