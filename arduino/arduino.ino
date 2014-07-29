#include <Wire.h>
#include "i2c_interface.h"
#include "motors.h"
#include "encoders.h"

struct
{
  uint8_t motors_modified;
  int16_t motors[2];
} data;

struct
{
  int8_t count1;
  int8_t count2;
  uint32_t errors1;
  uint32_t errors2;
} report;

void setup()
{
  pinMode(13, OUTPUT);
  
  I2cInterface::setup(20, (uint8_t *)(&data), sizeof(data),
    (uint8_t *)(&report), sizeof(report));
  Motors::setup(4,5);
  Encoders::setup();
}

void loop()
{
  Motors::set(data.motors[0],data.motors[1]);
}

