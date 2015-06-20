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

SlaveCommand(LEDs, bool yellow; bool green; bool red;)
void LEDs::run()
{
  ledYellow(yellow);
  ledGreen(green);
  ledRed(red);
}

SlaveCommand(Play, char notes[126];)

void Play::run()
{
  buzzer.play(notes);
  while(buzzer.isPlaying());
}

SlaveCommand(SetMotors, short left; short right;)

void SetMotors::run()
{
  motors.set(left, right);
}

void DemoSlave::handleSlaveCommand(uint8_t cmd, const void *args)
{
  switch(cmd)
  {
    case 2:
      Cmd<SetMotors>::run(args);
      break;
    case 3:
      Cmd<Play>::run(args);
      break;
    case 4:
      Cmd<LEDs>::run(args);
      break;
  }
}
