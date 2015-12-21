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

struct Data
{
  bool yellow, green, red;
  bool buttonA, buttonB, buttonC;
  int16_t leftMotor, rightMotor;
  uint16_t batteryMillivolts;
  uint16_t analog[6];
};

void loop()
{
  slave.updateUserBuffer();
  struct Data *data = (struct Data *)slave.user_buffer;
  
  setLEDs(data->yellow, data->green, data->red);
  data->buttonA = buttonA.isPressed();
  data->buttonB = buttonB.isPressed();
  data->buttonC = buttonC.isPressed();
  setMotors(data->leftMotor, data->rightMotor);

  data->batteryMillivolts = readBatteryMillivoltsLV();

  for(uint8_t i=0; i<6; i++)
  {
    data->analog[i] = analogRead(i);
  }
  
  slave.finishUserWrites();
}

