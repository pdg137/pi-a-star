#pragma once
#include "RPiSlave.h"

class DemoSlave: public RPiSlave
{
  SlaveCommand(SetMotors, short left; short right;)
  SlaveCommand(Play, char notes[ARGS_LENGTH];)
  SlaveCommand(LEDs, bool yellow; bool green; bool red;)
  
public:
//  MasterCommand(Print, char message[ARGS_LENGTH]);
  struct Print { char message[ARGS_LENGTH]; };
  bool run(Print args)
  {
    masterCommand(1, (void *)&args, sizeof(args));
  }

  virtual void init();
  virtual void handleSlaveCommand(uint8_t cmd, const void *args);
};
