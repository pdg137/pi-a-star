#include "Arduino.h"
#include "RpiSlave.h"
#include "FastTWISlave.h"

void RPiSlave::finalizeI2CWrites()
{
  uint8_t i;
  for(i=0; i < BUFFER_LENGTH; i++)
  {
    if(i2c_buffer_old[i] != i2c_buffer[i])
      i2c_buffer_old[i] = staging_buffer[i] = i2c_buffer[i];
  }
}

void RPiSlave::updateI2CBuffer()
{
  memcpy(i2c_buffer, staging_buffer, BUFFER_LENGTH);
  memcpy(i2c_buffer_old, staging_buffer, BUFFER_LENGTH);
}

void RPiSlave::finalizeWrites()
{
  cli();
  uint8_t i;
  for(i=0; i < BUFFER_LENGTH; i++)
  {
    if(buffer_old[i] != buffer[i])
      buffer_old[i] = staging_buffer[i] = buffer[i];
  }
  sei();
}

void RPiSlave::updateBuffer()
{
  cli();
  memcpy(buffer, staging_buffer, BUFFER_LENGTH);
  memcpy(buffer_old, staging_buffer, BUFFER_LENGTH);
  sei();
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
    updateI2CBuffer();
    index = b;
    index_set = 1;
  }
  else
  {
    // Write the data to the buffer
    i2c_buffer[index] = b;
    index ++;

    // Wrap at the end of the buffer
    if(index > sizeof(i2c_buffer))
      index = 0;
  }
}

void RPiSlave::start()
{
  piDelay();
  index_set = 0;
}

void RPiSlave::stop()
{
  finalizeI2CWrites();
}

unsigned char RPiSlave::transmit()
{
  piDelay();
  return ((char *)&i2c_buffer)[index++];
}
  
void RPiSlave::init(unsigned char address)
{
  FastTWISlave::init(address, *this);
}
