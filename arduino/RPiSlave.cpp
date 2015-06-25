#include "Arduino.h"
#include "RpiSlave.h"
#include "FastTWISlave.h"

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
    // Write the data to the buffer
    ((char *)&data)[index] = b;
    index ++;

    // Write the lock value to status if we wrote to the command area.
    if(index < 128)
      data.command_status = CMD_STATUS_LOCK;
  }
}

void RPiSlave::start()
{
  piDelay();
  index = 0; // go to the beginning of the array
  index_set = 0;
}

void RPiSlave::stop()
{
  // Sets the status to "call" if it is in "lock".
  if(CMD_STATUS_LOCK == data.command_status)
    data.command_status = CMD_STATUS_CALL;
}

unsigned char RPiSlave::transmit()
{
  piDelay();
  return ((char *)&data)[index++];
}
  
void RPiSlave::init(unsigned char address)
{
  FastTWISlave::init(address, *this);
}

void RPiSlave::loop()
{
  if(CMD_STATUS_CALL == data.command_status)
  {
    handleSlaveCommand(data.command_number, data.args);
    data.command_status = CMD_STATUS_RETURN;
  }
}
