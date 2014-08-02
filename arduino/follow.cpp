#include "Arduino.h"
#include "follow.h"
#include "motors.h"
#include "encoders.h"
#include "buttons.h"

uint16_t calibration_min[5] = {40,680,500,680,40};
uint16_t calibration_max[5] = {960, 920, 920, 920, 960};

#define limit(x,a,b) (max(min((x),(b)),(a)))

uint8_t follow_line;
int16_t last_pos;
uint8_t off_line;
int32_t off_line_distance;
uint8_t detect_left_count;
uint8_t detect_right_count;
uint8_t detected_intersection;
int32_t detected_intersection_distance;

uint8_t Follow::sensors[5];
int16_t Follow::pos;
uint8_t Follow::detected_left, Follow::detected_straight, Follow::detected_right;
  
void Follow::update()
{
  uint8_t i;
  for(i=0;i<5;i++)
  {
    Follow::sensors[i] = (limit(analogRead(i+1),calibration_min[i],calibration_max[i])-calibration_min[i])*(uint32_t)255/(calibration_max[i]-calibration_min[i]);
  }
  
  if(max(max(Follow::sensors[1],Follow::sensors[2]),Follow::sensors[3]) < 20)
  {
    // off line
    Follow::pos = 0;
    if(!off_line)
    {
      off_line = 1;
      off_line_distance = Encoders::distance;
    }
  }
  else
  {
    off_line = 0;
    Follow::pos = (-(int32_t)sensors[1]+sensors[3])*1000/(sensors[1]+sensors[2]+sensors[3]);
  }
  
  if(sensors[0] > 128)
    detect_left_count += 1;
  else
    detect_left_count = 0;
  if(detect_left_count > 10) // maybe add min distance
    detected_left = 1;
  if(sensors[4] > 128)
    detect_right_count += 1;
  else
    detect_right_count = 0;
  if(detect_right_count > 10)
    detected_right = 1;
  
  if((Follow::detected_left || Follow::detected_right) &&
    !detected_intersection)
  {
    detected_intersection = 1;
    detected_intersection_distance = Encoders::distance;
  }
  
  if(off_line && Encoders::distance - off_line_distance > 600 ||
    detected_intersection && Encoders::distance - detected_intersection_distance > 600)
  {
    Follow::detected_straight = !off_line;
    follow_line = 0;
  }
  
  if(Buttons::button1)
  {
    off_line = 0;
    detected_intersection = 0;
    Follow::detected_left = Follow::detected_straight = Follow::detected_right = 0;
    follow_line = 1;
  }
  
  if(follow_line)
  {
    int32_t diff = last_pos - Follow::pos;
    
    int32_t pid = Follow::pos/4 - diff;
    Motors::set(limit(100+pid,0,100), limit(100-pid,0,100));
    
    last_pos = Follow::pos;
  }
  else
  {
    Motors::set(0,0);
  }
}
