#pragma once
#include "RPiSlave.h"

class DemoSlave: public RPiSlave
{
public:
  void masterPrint(char (&message)[ARGS_LENGTH]);

  virtual void init();
  virtual void handleSlaveCommand();
};
