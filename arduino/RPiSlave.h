#pragma once
#include "FastTWISlave.h"

const uint8_t ARGS_LENGTH = 32;

static unsigned char CMD_STATUS=0;
static unsigned char CMD_NUMBER=1;
static unsigned char CMD_STATUS_LOCK=2;
static unsigned char CMD_STATUS_CALL=1;
static unsigned char CMD_STATUS_RETURN=0;

#define SlaveCommand(name, args) struct __attribute__ ((__packed__)) name { args void run(); }; \
  void run ## name(const void * data) \
  { \
    Cmd<name>::run(data); \
  }

class RPiSlave: public FastTWISlave
{
private:

template <class T>
  class Cmd {
  public:
    static bool run(const void *args)
    {
      ((T *)args)->run();
    }
  };

  struct CommandData
  {
    uint8_t status, command;
    char args[ARGS_LENGTH];
  };

  struct Data
  {
    CommandData slaveCommand;
    CommandData masterCommand;
    char scratch[ARGS_LENGTH];
  } data;
  unsigned char index;
  unsigned char index_set = 0;

  void piDelay();

public:
  virtual void handleSlaveCommand(uint8_t cmd, const void *args);
  bool masterCommand(uint8_t command, void *args, size_t len);
  void loop();

  virtual void receive(uint8_t b);
  virtual uint8_t transmit();
  virtual void start();
  virtual void stop();

  /* Initialize the slave on a given address. */
  void init(unsigned char address);
};
