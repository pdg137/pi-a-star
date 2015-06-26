#pragma once
#include "RPiSlave.h"

class DemoSlave: public RPiSlave
{
  SlaveCommand(2, SetMotors, short left; short right;);
  SlaveCommand(3, Play, char notes[ARGS_LENGTH];);
  SlaveCommand(4, LEDs, bool yellow; bool green; bool red;);
  
public:
  MasterCommand(1, Print, char message[ARGS_LENGTH];);

  virtual void init();
  virtual void handleSlaveCommand();
};
