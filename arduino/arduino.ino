#include <Servo.h>
#include <AStar32U4Prime.h>

#include "DemoSlave.h"
DemoSlave slave;

void setup()
{
  slave.init();
}

unsigned long last_print = 0;

void loop()
{
  slave.loop();
  if(millis() - last_print > 1000)
  {
    slave.run(DemoSlave::Print {"Hello, world!\n"});
    last_print += 1000;
  }
}

