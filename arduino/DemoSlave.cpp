#include <AStar32U4Prime.h>
#include "DemoSlave.h"
#include "motors.h"

PololuBuzzer buzzer;
Motors motors;

void DemoSlave::init()
{
  motors.setup(8);
  RPiSlave::init(20);
  buzzer.play("v10>>l16g>>>c");
}

uint16_t getBatteryVoltage()
{
  return readBatteryMillivoltsLV();
}

void setLEDs(bool yellow, bool green, bool red)
{
  ledYellow(yellow);
  ledGreen(green);
  ledRed(red);
}


void playNotes(char *notes)
{
  buzzer.play(notes);
  while(buzzer.isPlaying());
}

void setMotors(int16_t left, int16_t right)
{
  motors.set(left, right);
}

void DemoSlave::masterPrint(char (&message)[ARGS_LENGTH])
{
  runMasterCommand(1, message);
}

void DemoSlave::handleSlaveCommand()
{
  checkCommand(1,getBatteryVoltage);
  checkCommand(2,setMotors);
  checkCommand(3,playNotes);
  checkCommand(4,setLEDs);
}
