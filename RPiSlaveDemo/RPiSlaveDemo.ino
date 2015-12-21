#include <Servo.h>
#include <AStar32U4.h>
#include "RPiSlave.h"

RPiSlave slave;
PololuBuzzer buzzer;
AStar32U4Motors motors;
AStar32U4ButtonA buttonA;
AStar32U4ButtonB buttonB;
AStar32U4ButtonC buttonC;

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

void loop()
{
  slave.updateUserBuffer();
  setLEDs(slave.user_buffer[0],slave.user_buffer[1],slave.user_buffer[2]);
  slave.user_buffer[3] = buttonA.isPressed();
  slave.user_buffer[4] = buttonB.isPressed();
  slave.user_buffer[5] = buttonC.isPressed();
  setMotors(*(int16_t *)(slave.user_buffer+6), *(int16_t *)(slave.user_buffer + 8));
  slave.finishUserWrites();
}

