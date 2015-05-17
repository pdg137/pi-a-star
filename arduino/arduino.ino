#include "RPiSlave.h"

void setup()
{
  pinMode(13, OUTPUT);
  RPiSlave::init(20);
}

void loop()
{
  if(1 == RPiSlave::checkForCommand())
  {
    unsigned char tmp = RPiSlave::getByte(2);
    RPiSlave::setByte(2, RPiSlave::getByte(3));
    RPiSlave::setByte(3, tmp);
    RPiSlave::commandReturn();
  }
}

