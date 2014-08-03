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
int32_t turn_goal;
int32_t state_start_millis;

uint8_t Follow::sensors[5];
int16_t Follow::pos;
uint8_t Follow::detected_left, Follow::detected_straight, Follow::detected_right;
uint8_t Follow::state;

void Follow::readSensors()
{ 
  uint8_t i;
  for(i=0;i<5;i++)
  {
    Follow::sensors[i] = (limit(analogRead(i+1),calibration_min[i],calibration_max[i])-calibration_min[i])*(uint32_t)255/(calibration_max[i]-calibration_min[i]);
  }
  
  if(max(max(Follow::sensors[1],Follow::sensors[2]),Follow::sensors[3]) < 20)
  {
    // off line
    if(Encoders::distance > 1000)
    {
      // probably at end
      Follow::pos = 0;
    }
    else
    {
      if(pos > 0)
        pos = 1000;
      else
        pos = -1000;
    }
    
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
}

void Follow::update()
{
  static uint8_t last_state = STATE_NONE;
  
  readSensors();
  
  if(state != last_state)
  {
    state_start_millis = millis();
    last_state = state;
  }
  
  switch(state)
  {
  case STATE_FOLLOWING:
    follow();
    break;
  case STATE_WAITING:
    wait();
    break;
  case STATE_TURNING:
    turn();
    break;
  case STATE_SNAPPING:
    snap();
    break;
  }
}

void Follow::doTurn(int16_t angle_degrees)
{
  Encoders::reset();
  turn_goal = angle_degrees*12;
  state = STATE_TURNING;
}

void Follow::turn()
{
  if(turn_goal > 0)
  {
    Motors::set(100,-100);
  
    if(Encoders::turn >= turn_goal)
      state = STATE_SNAPPING;
  }
  else
  {
    Motors::set(-100,100);
    
    if(Encoders::turn <= turn_goal)
      state = STATE_SNAPPING;
  }
}

void Follow::doFollow()
{ 
  Encoders::reset();
  off_line = 0;
  detected_intersection = 0;
  detected_left = detected_straight = detected_right = 0;
  state = STATE_FOLLOWING;
}

void Follow::wait()
{
  Motors::set(0,0);
}

void Follow::snap()
{
  int32_t diff = last_pos - pos;
  int32_t pid = pos/4 - diff;
  Motors::set(limit(pid,-100,100), limit(-pid,-100,100));
  last_pos = pos;
  
  if(millis() - state_start_millis > 200)
    state = STATE_WAITING;
}

void Follow::follow()
{  
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
  
  if((detected_left || detected_right) &&
    !detected_intersection)
  {
    detected_intersection = 1;
    detected_intersection_distance = Encoders::distance;
  }
  
  if(off_line && Encoders::distance - off_line_distance > 600 ||
    detected_intersection && Encoders::distance - detected_intersection_distance > 600)
  {
    detected_straight = !off_line;
    state = STATE_WAITING;
  }
  
  int32_t diff = last_pos - pos;
  int32_t pid = pos/4 - diff;
  Motors::set(limit(100+pid,0,100), limit(100-pid,0,100));
  last_pos = pos;
}
