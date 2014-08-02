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
  int32_t distance;
  uint32_t error1;
  uint32_t error2;
  uint8_t button_state;
  uint8_t sensors[5];
  int16_t pos;
  uint8_t detected_left, detected_right;
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

uint8_t follow_line;
int16_t last_pos;
uint8_t off_line;
int32_t off_line_distance;
uint8_t detect_left_count;
uint8_t detect_right_count;
int32_t detected_left_distance;
int32_t detected_right_distance;

void loop()
{
  Encoders::update();
  report.distance = Encoders::distance;
  report.error1 = Encoders::error1;
  report.error2 = Encoders::error2;
  
  uint8_t i;
  for(i=0;i<5;i++)
  {
    report.sensors[i] = (limit(analogRead(i+1),calibration_min[i],calibration_max[i])-calibration_min[i])*(uint32_t)255/(calibration_max[i]-calibration_min[i]);
  }
  
  if(max(max(report.sensors[1],report.sensors[2]),report.sensors[3]) < 20)
  {
    // off line
    report.pos = 0;
    if(!off_line)
    {
      off_line = 1;
      off_line_distance = Encoders::distance;
    }
  }
  else
  {
    off_line = 0;
    report.pos = (-(int32_t)report.sensors[1]+report.sensors[3])*1000/(report.sensors[1]+report.sensors[2]+report.sensors[3]);
  }
  
  if(report.sensors[0] > 128)
    detect_left_count += 1;
  else
    detect_left_count = 0;
  if(detect_left_count > 10)
    detected_left_distance = Encoders::distance;
  if(report.sensors[4] > 128)
    detect_right_count += 1;
  else
    detect_right_count = 0;
  if(detect_right_count > 10)
    detected_right_distance = Encoders::distance;
  
  if(off_line && Encoders::distance - off_line_distance > 600)
  {
    follow_line = 0;
    
    // check for exits
    report.detected_left = (Encoders::distance - detected_left_distance < 1000);
    report.detected_right = (Encoders::distance - detected_right_distance < 1000);
  }
  
  report.button_state = (digitalRead(18) ? 0 : 1) +
    (digitalRead(15) ? 0 : 2) +
    (digitalRead(14) ? 0 : 4);
  
  if(report.button_state & 2)
    follow_line = 1;
  
  if(follow_line)
  {
    int32_t diff = last_pos - report.pos;
    
    int32_t pid = report.pos/4 - diff;
    Motors::set(limit(100+pid,0,100), limit(100-pid,0,100));
    
    last_pos = report.pos;
    report.detected_left = report.detected_right = 0;
  }
  else
  {
    off_line = 0;
    detected_left_distance = detected_right_distance = 0;
    Motors::set(0,0);
  }

  if(data.led_state & 1) RXLED1;
  else RXLED0;
  
  if(data.led_state & 2) digitalWrite(13, HIGH);
  else digitalWrite(13, LOW);
  
  if(data.led_state & 4) TXLED1;
  else TXLED0;
}

