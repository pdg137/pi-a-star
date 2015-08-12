#include <Servo.h>
#include <AStar32U4.h>
#include "RPiSlave.h"

RPiSlave slave;
PololuBuzzer buzzer;
AStar32U4Motors motors;

void setup()
{
  slave.init(20);
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
  motors.setSpeeds(left, right);
}

void masterPrint(char (&message)[ARGS_LENGTH])
{
  slave.runMasterCommand(1, message);
}

unsigned long last_print = 0;

void checkCommands()
{
  slave.checkCommand(1, getBatteryVoltage);
  slave.checkCommand(2, setMotors);
  slave.checkCommand(3, playNotes);
  slave.checkCommand(4, setLEDs);
}

void loop()
{
  if(slave.commandReady())
  {
    checkCommands();
    slave.commandDone();
  }
  if(millis() - last_print > 1000)
  {
    char buf[ARGS_LENGTH];
    snprintf(buf, sizeof(buf), "Hello, world! %ds\n", last_print/1000);
    masterPrint(buf);
    last_print += 1000;
  }
}

