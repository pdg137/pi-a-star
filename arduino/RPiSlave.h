#pragma once
#include "FastTWISlave.h"

static unsigned char CMD_STATUS=0;
static unsigned char CMD_NUMBER=1;
static unsigned char CMD_STATUS_LOCK=2;
static unsigned char CMD_STATUS_CALL=0;
static unsigned char CMD_STATUS_RETURN=1;

template <class T>
class Cmd {
public:
  static bool run(const void *args)
  {
    ((T *)args)->run();
  }
};

#define SlaveCommand(name, args) struct __attribute__ ((__packed__)) name {args void run();};

class RPiSlave: public FastTWISlave
{
  void piDelay();

public:
  virtual void handleSlaveCommand(uint8_t cmd, const void *args);
  void loop();

  virtual void receive(uint8_t b);
  virtual uint8_t transmit();
  virtual void start();
  virtual void stop();

  /* Initialize the slave on a given address. */
  void init(unsigned char address);

  /* Get a byte from the buffer */
  unsigned char getByte(unsigned char index);

  /* Get a pointer to a string starting at a given position */
  const char *getString(unsigned char index); 

  const char *getArguments() { return getString(2); }

  /* Get an int from the buffer */
  short getInt16(unsigned char index);

  /* Set a byte in the buffer */
  void setByte(unsigned char index, unsigned char value);
  
  /* Check for a command from RPi and return the command # if we are being called. */
  unsigned char checkForCommand();
  
  /* Indicate to the RPi that we are done processing a command. */
  void commandReturn();

private:
  char data[256];
  unsigned char index;
  unsigned char index_set = 0;
};
