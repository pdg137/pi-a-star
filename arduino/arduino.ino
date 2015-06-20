#include <Servo.h>
#include <AStar32U4Prime.h>

#include "RPiSlave.h"
#include "motors.h"

Motors motors;
RPiSlave rpi_slave;

void setup()
{
  motors.setup(8);
  pinMode(13, OUTPUT);
  rpi_slave.init(20);
  delay(1000);
}

void loop()
{
  unsigned char tmp;
  switch(rpi_slave.checkForCommand())
  {
  case 2:
    motors.set(rpi_slave.getInt16(2),rpi_slave.getInt16(4));
    rpi_slave.commandReturn();
    break;
  }
  
  digitalWrite(13, millis() % 256 < 128);
}

