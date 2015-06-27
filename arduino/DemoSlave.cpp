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

void DemoSlave::GetBatteryVoltage::run()
{
  battery_voltage_mv = readBatteryMillivoltsLV();
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

void DemoSlave::handleSlaveCommand()
{
  checkCommand<SetMotors>();
  checkCommand<Play>();
  checkCommand<LEDs>();
  checkCommand<GetBatteryVoltage>();
}
