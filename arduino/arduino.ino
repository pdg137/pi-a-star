#include <Wire.h>
#include "i2c_interface.h"
#include "motors.h"

struct
{
  uint8_t motors_modified;
  int16_t motors[2];
} data;

void setup()
{
  pinMode(13, OUTPUT);
  
  I2cInterface::setup(20, (uint8_t *)(&data), sizeof(data));
  Motors::setup(4,5);
}

void loop()
{
  Motors::set(data.motors[0],data.motors[1]);
}

