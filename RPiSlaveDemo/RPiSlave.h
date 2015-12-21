#pragma once
#include "FastTWISlave.h"

const uint8_t BUFFER_LENGTH = 32;

class RPiSlave: public FastTWISlave
{
private:
  char i2c_buffer[BUFFER_LENGTH];
  char i2c_buffer_old[BUFFER_LENGTH];

  char staging_buffer[BUFFER_LENGTH];

  unsigned char index;
  unsigned char index_set = 0;

  void piDelay();

public:  
  char user_buffer_old[BUFFER_LENGTH];
  char user_buffer[BUFFER_LENGTH];

  void updateUserBuffer();
  void finishUserWrites();
  void updateI2CBuffer();
  void finishI2CWrites();

  virtual void receive(uint8_t b);
  virtual uint8_t transmit();
  virtual void start();
  virtual void stop();

  /* Initialize the slave on a given address. */
  void init(unsigned char address);
};
