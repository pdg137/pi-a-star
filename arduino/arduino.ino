#include <Servo.h>
#include <AStar32U4Prime.h>

#include "DemoSlave.h"
DemoSlave slave;

void setup()
{
  slave.init();
}

void loop()
{ 
  slave.loop();
}

