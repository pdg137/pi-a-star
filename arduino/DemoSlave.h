#pragma once
#include "RPiSlave.h"

class DemoSlave: public RPiSlave
{
public:
  virtual void init();
  virtual void handleSlaveCommand(uint8_t cmd, const void *args);
};
