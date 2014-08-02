#include <Wire.h>
#include "i2c_interface.h"
#include "motors.h"
#include "encoders.h"

struct
{
  uint8_t motors_modified;
  int16_t motors[2];
  uint8_t led_state_modified;
  uint8_t led_state;
} data;

struct
{
  int8_t count1;
  int8_t count2;
  uint32_t error1;
  uint32_t error2;
  uint8_t button_state;
  uint8_t sensors[5];
} report;


uint16_t calibration_min[5] = {40,680,500,680,40};
uint16_t calibration_max[5] = {960, 920, 920, 920, 960};

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(18, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  
  I2cInterface::setup(20, (uint8_t *)(&data), sizeof(data),
    (uint8_t *)(&report), sizeof(report));
  Motors::setup(4,5);
  Encoders::setup();
}

#define limit(x,a,b) (max(min((x),(b)),(a)))

void loop()
{
  report.count1 = Encoders::count1;
  report.count2 = Encoders::count2;
  report.error1 = Encoders::error1;
  report.error2 = Encoders::error2;
  
  uint8_t i;
  for(i=0;i<5;i++)
  {
    report.sensors[i] = (limit(analogRead(i+1),calibration_min[i],calibration_max[i])-calibration_min[i])*(uint32_t)255/(calibration_max[i]-calibration_min[i]);
  }
  
  report.button_state = (digitalRead(18) ? 0 : 1) +
    (digitalRead(15) ? 0 : 2) +
    (digitalRead(14) ? 0 : 4);
  
  Motors::set(data.motors[0],data.motors[1]);

  if(data.led_state & 1) RXLED1;
  else RXLED0;
  
  if(data.led_state & 2) digitalWrite(13, HIGH);
  else digitalWrite(13, LOW);
  
  if(data.led_state & 4) TXLED1;
  else TXLED0;
}

