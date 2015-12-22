#include <Servo.h>
#include <AStar32U4.h>
#include "RPiSlave.h"

/* This example program shows how to make the A-Star 32U4 Robot
 * Controller into a Raspberry Pi I2C slave.  The RPi and A-Star can
 * exchange data bidirectionally, allowing each device to do what it
 * does best: high-level programming can be handled in a language such
 * as Python on the RPi, while the A-Star is takes charge of motor
 * control, analog inputs, and other low-level I/O.
 *
 * See code in the pi/ subfolder for the corresponding master
 * implementation, a simple Python-based web application that creates
 * a mobile-friendly control panel for a Raspberry Pi robot.
 */

RPiSlave slave;
PololuBuzzer buzzer;
AStar32U4Motors motors;
AStar32U4ButtonA buttonA;
AStar32U4ButtonB buttonB;
AStar32U4ButtonC buttonC;

void setup()
{
  // Set up the slave at I2C address 20.
  slave.init(20);

  // Play startup sound.
  buzzer.play("v10>>l16g>>>c");
}

void setLEDs(bool yellow, bool green, bool red)
{
  ledYellow(yellow);
  ledGreen(green);
  ledRed(red);
}

void playNotes(const char *notes)
{
  buzzer.play(notes);
  while(buzzer.isPlaying());
}

void setMotors(int16_t left, int16_t right)
{
  motors.setSpeeds(left, right);
}

// Custom data structure that we will use for interpreting the buffer.
// Use at most 32 bytes total unless you change BUFFER_LENGTH in
// RPiSlave.h.
struct Data
{
  // 6 bytes
  bool yellow, green, red;
  bool buttonA, buttonB, buttonC;

  // 9*2 = 18 bytes 
  int16_t leftMotor, rightMotor;
  uint16_t batteryMillivolts;
  uint16_t analog[6];

  // 1 byte
  bool playNotes;

  // 7 bytes left to get to 32
  char notes[7];
};

void loop()
{
  // Call updateBuffer() before using the buffer, to get the latest
  // data including recent master writes.
  slave.updateBuffer();

  // Cast the buffer to our custom data structure for easy access to
  // named and multi-byte values.  (The buffer never moves, so we
  // could do this in setup(), but we left it here for clarity.)
  struct Data *data = (struct Data *)slave.buffer;

  // Write various values into the data structure.
  data->buttonA = buttonA.isPressed();
  data->buttonB = buttonB.isPressed();
  data->buttonC = buttonC.isPressed();

  data->batteryMillivolts = readBatteryMillivoltsLV();

  for(uint8_t i=0; i<6; i++)
  {
    data->analog[i] = analogRead(i);
  }

  // READING the buffer is allowed before or after finishWrites().
  setLEDs(data->yellow, data->green, data->red);
  setMotors(data->leftMotor, data->rightMotor);

  // Playing music involves both reading and writing, since we only
  // want to do it once:
  if(data->playNotes)
  {
    playNotes(data->notes);
    data->playNotes = false;
  }

  // When you are done WRITING, call finalizeWrites() to make modified
  // data available to I2C master.
  slave.finalizeWrites();
}
