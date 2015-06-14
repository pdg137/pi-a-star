#include <Servo.h>

#include "RPiSlave.h"
#include "motors.h"

Motors motors;

void setup()
{
  motors.setup(16,15,4);
  pinMode(13, OUTPUT);
  RPiSlave::init(20);
  delay(1000);
}

void loop()
{
  unsigned char tmp;
  switch(RPiSlave::checkForCommand())
  {
  case 1:
    tmp = RPiSlave::getByte(2);
    RPiSlave::setByte(2, RPiSlave::getByte(3));
    RPiSlave::setByte(3, tmp);
    RPiSlave::commandReturn();
    break;
  case 2:
    motors.set(RPiSlave::getInt16(2),RPiSlave::getInt16(4));
    break;
  }
  
  digitalWrite(13, millis() % 256 < 128);
}

