#include <Servo.h>
#include <AStar32U4Prime.h>

#include "RPiSlave.h"
#include "motors.h"

Motors motors;
RPiSlave rpi_slave;
PololuBuzzer buzzer;

void setup()
{
  motors.setup(8);
  pinMode(13, OUTPUT);
  rpi_slave.init(20);
}

void loop()
{
  unsigned char tmp;
  
  switch(rpi_slave.checkForCommand())
  {
  case 2: // MOTORS
    motors.set(rpi_slave.getInt16(2),rpi_slave.getInt16(4));
    rpi_slave.commandReturn();
    break;
  case 3: // PLAY
    buzzer.play(rpi_slave.getString(2));
    while(buzzer.isPlaying());
    rpi_slave.commandReturn();
    break;
  case 4: // LEDS
    ledYellow(rpi_slave.getByte(2));
    ledGreen(rpi_slave.getByte(3));
    ledRed(rpi_slave.getByte(4));
    break;
  }
}

