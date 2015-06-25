#pragma once
#include "RPiSlave.h"

class DemoSlave: public RPiSlave
{
  SlaveCommand(SetMotors, short left; short right;)
  SlaveCommand(Play, char notes[126];)
  SlaveCommand(LEDs, bool yellow; bool green; bool red;)
  
public:
  virtual void init();
  virtual void handleSlaveCommand(uint8_t cmd, const void *args);
};
