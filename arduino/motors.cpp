#include "Arduino.h"
#include "motors.h"

void Motors::set(int left, int right)
{
  if(left < 0)
  {
    digitalWrite(left_dir_pin, LOW);
    OCR1A = min(-left, 400);
  }
  else
  {
    digitalWrite(left_dir_pin, HIGH);
    OCR1A = min(left, 400);
  }
  
  if(right < 0)
  {
    digitalWrite(right_dir_pin, LOW);
    OCR1B = min(-right, 400);
  }
  else
  {
    digitalWrite(right_dir_pin, HIGH);
    OCR1B = min(right, 400);
  }
}

uint8_t Motors::left_dir_pin;
uint8_t Motors::right_dir_pin;

void Motors::setup(uint8_t left_dir_pin, uint8_t right_dir_pin)
{
  Motors::left_dir_pin = left_dir_pin;
  Motors::right_dir_pin = right_dir_pin;
  // 20kHz PWM copied from Zumo shield library
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(left_dir_pin, OUTPUT);
  pinMode(right_dir_pin, OUTPUT);
  Motors::set(0,0);
}
