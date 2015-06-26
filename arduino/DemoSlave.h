#pragma once
#include "RPiSlave.h"

class DemoSlave: public RPiSlave
{
  SlaveCommand(SetMotors, short left; short right;);
  SlaveCommand(Play, char notes[ARGS_LENGTH];);
  SlaveCommand(LEDs, bool yellow; bool green; bool red;);
  
public:
  MasterCommand(Print, 1, char message[ARGS_LENGTH];);

  virtual void init();
  virtual void handleSlaveCommand(uint8_t cmd);
};
