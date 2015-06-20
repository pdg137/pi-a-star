#include <Servo.h>
#include <FastGPIO.h>

#include "Arduino.h"
#include "motors.h"

#define left_dir_pin 12
#define right_dir_pin IO_E2

Servo servo;

void Motors::set(int left, int right)
{
  if(left < 0)
  {
    FastGPIO::Pin<left_dir_pin>::setOutputValueHigh();
    OCR1A = min(-left, 400);
  }
  else
  {
    FastGPIO::Pin<left_dir_pin>::setOutputValueLow();
    OCR1A = min(left, 400);
  }
  
  if(right < 0)
  {
    FastGPIO::Pin<right_dir_pin>::setOutputValueHigh();
    OCR1B = min(-right, 400);
  }
  else
  {
    FastGPIO::Pin<right_dir_pin>::setOutputValueLow();
    OCR1B = min(right, 400);
  }
  
  Motors::setServo(left, right);
}

void Motors::setServo(int left, int right)
{
  // x x -> 90
  // x -x -> 170 / 25
  // 0 x -> 60
  // x 0 -> 120
  
  if(left + right == 0)
  {
    servo.write(170);
    return;
  }

  int ratio = 40 * (left - right) / (left + right);
  // with this 40:1 scale, the max value is 40*800/1 = 32000,
  // barely fitting in an int.
  if(ratio < -40)
  {
    servo.write(60-(ratio+40)/914);
    return;
  }
  if(ratio < 40)
  {
    servo.write(90+ratio*30/80);
    return;
  }
  
  servo.write(120+(ratio-40)/640);
}

void Motors::setup(unsigned char servo_pin)
{
  // 20kHz PWM copied from Zumo shield library
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  Motors::set(0,0);
  servo.attach(servo_pin);
}
