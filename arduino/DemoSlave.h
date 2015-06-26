#pragma once
#include "RPiSlave.h"

class DemoSlave: public RPiSlave
{
  SlaveCommand(SetMotors, short left; short right;)
  SlaveCommand(Play, char notes[ARGS_LENGTH];)
  SlaveCommand(LEDs, bool yellow; bool green; bool red;)
  
public:
//  MasterCommand(Print, char message[ARGS_LENGTH]);
  struct Print {
    char message[ARGS_LENGTH];
    static uint8_t command() { return 1; }
  };

  virtual void init();
  virtual void handleSlaveCommand(uint8_t cmd);
};
