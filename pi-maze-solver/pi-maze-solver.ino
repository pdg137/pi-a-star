#include <Wire.h>
#include "i2c_interface.h"

void setMotors(int left, int right)
{
  if(left < 0)
  {
    digitalWrite(4, LOW);
    OCR1A = min(-left, 400);
  }
  else
  {
    digitalWrite(4, HIGH);
    OCR1A = min(left, 400);
  }
  
  if(right < 0)
  {
    digitalWrite(5, LOW);
    OCR1B = min(-right, 400);
  }
  else
  {
    digitalWrite(5, HIGH);
    OCR1B = min(right, 400);
  }
}

struct
{
  uint8_t motors_modified;
  int16_t motors[2];
} data;

void setup()
{
  pinMode(13, OUTPUT);
  
  I2cInterface::setupReceive(20, (uint8_t *)(&data), sizeof(data));
  
  // 20kHz PWM copied from Zumo shield library
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  setMotors(0,0);
}

void loop()
{
//  if(bytesAvailable())
//    readByte();

  
  setMotors(data.motors[0],data.motors[1]);
}

