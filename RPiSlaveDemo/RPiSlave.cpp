#include "Arduino.h"
#include "RpiSlave.h"
#include "FastTWISlave.h"

/*
 * We will have three buffers: i2c, staging, and user.  The idea is that
 * reads and writes involving i2c and user are asynchronous and slow, and
 * you can only safely copy data when there is no reading/writing
 * happening, so without the staging buffer there might never be a safe
 * time to do copies.
 * 
 * We have two additional arrays to keep track of the original values as
 * of the last sync time, so that we know what bytes were written and need
 * to be copied.  Without these additional arrays, bidirectional
 * simultaneous writes would not be possible.
 * 
 */

void RPiSlave::finishI2CWrites()
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

void RPiSlave::finishUserWrites()
{
  cli();
  uint8_t i;
  for(i=0; i < BUFFER_LENGTH; i++)
  {
    if(user_buffer_old[i] != user_buffer[i])
      user_buffer_old[i] = staging_buffer[i] = user_buffer[i];
  }
  sei();
}

void RPiSlave::updateUserBuffer()
{
  cli();
  memcpy(user_buffer, staging_buffer, BUFFER_LENGTH);
  memcpy(user_buffer_old, staging_buffer, BUFFER_LENGTH);
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
  finishI2CWrites();
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
