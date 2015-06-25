#include <AStar32U4Prime.h>
#include "DemoSlave.h"
#include "motors.h"

PololuBuzzer buzzer;
Motors motors;

void DemoSlave::init()
{
  motors.setup(8);
  RPiSlave::init(20);
}

void DemoSlave::LEDs::run()
{
  ledYellow(yellow);
  ledGreen(green);
  ledRed(red);
}

void DemoSlave::Play::run()
{
  buzzer.play(notes);
  while(buzzer.isPlaying());
}

void DemoSlave::SetMotors::run()
{
  motors.set(left, right);
}

void DemoSlave::handleSlaveCommand(uint8_t cmd, const void *args)
{
  switch(cmd)
  {
    case 2:
      runSetMotors(args);
      break;
    case 3:
      runPlay(args);
      break;
    case 4:
      runLEDs(args);
      break;
  }
}
