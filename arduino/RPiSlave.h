#pragma once
#include "FastTWISlave.h"

const uint8_t ARGS_LENGTH = 32;

static unsigned char CMD_STATUS=0;
static unsigned char CMD_NUMBER=1;
static unsigned char CMD_STATUS_LOCK=2;
static unsigned char CMD_STATUS_CALL=1;
static unsigned char CMD_STATUS_RETURN=0;

#define SlaveCommand(cmd, name, args) struct __attribute__ ((__packed__)) name \
  { \
    args \
    static const uint8_t command = cmd; \
    void run(); \
  }

#define MasterCommand(cmd, name, args) struct __attribute__ ((__packed__))  name \
  { \
    args \
    static const uint8_t command = cmd; \
  }

class RPiSlave: public FastTWISlave
{
private:
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
  virtual void handleSlaveCommand();

  template<class T> T *startMasterCommand()
  {
    data.masterCommand.command = T::command;
    return (T *)(data.masterCommand.args);
  };

  void runMasterCommand();

  template<class T>
  void checkCommand()
  {
    if(T::command == data.slaveCommand.command)
      ((T *)data.slaveCommand.args)->run();
  };
  
  void loop();

  virtual void receive(uint8_t b);
  virtual uint8_t transmit();
  virtual void start();
  virtual void stop();

  /* Initialize the slave on a given address. */
  void init(unsigned char address);
};
