#include "Arduino.h"
#include "RpiSlave.h"
#include "FastTWISlave.h"

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

void RPiSlave::resetIndex()
{
  index = 0; // go to the beginning of the array
  index_set = 0;
}

// Write the lock value to status if we are about to write a command.
void RPiSlave::lockIfWritingCommand()
{
  if(index < 128)
    data[CMD_STATUS] = CMD_STATUS_LOCK;
}

// Sets the status to "call" if it is in "lock".
void RPiSlave::callIfLockedForCommand()
{
  if(CMD_STATUS_LOCK == data[0])
    data[CMD_STATUS] = CMD_STATUS_CALL;
}

// delay to accomodate the Broadcom I2C bug.
void RPiSlave::piDelay()
{
  delayMicroseconds(10);
}

void RPiSlave::receive(unsigned char b)
{
  piDelay();
  if(!index_set)
  {
    index = b;
    index_set = 1;
  }
  else
  {
    lockIfWritingCommand();
    data[index] = b;
    index ++;
  }
}

void RPiSlave::start()
{
  piDelay();
  resetIndex();
}

void RPiSlave::stop()
{
  callIfLockedForCommand();
}

unsigned char RPiSlave::transmit()
{
  piDelay();
  return data[index++];
}
  
void RPiSlave::init(unsigned char address)
{
  FastTWISlave::init(address, *this);
}
