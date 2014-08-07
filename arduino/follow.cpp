#include "Arduino.h"
#include "follow.h"
#include "motors.h"
#include "encoders.h"
#include "buttons.h"

#define SPEED 200
#define MAX_SPEED 400
#define MEDIUM_SPEED 300

#define LEFT_RIGHT_DETECT_COUNT 5

uint16_t calibration_min[5] = {40,680,500,680,40};
uint16_t calibration_max[5] = {960, 920, 920, 920, 960};

#define limit(x,a,b) (max(min((x),(b)),(a)))

uint8_t follow_line;
int16_t last_pos;
uint8_t off_line;
int32_t off_line_distance;
uint8_t on_line;
int32_t on_line_distance;
uint8_t detect_left_count;
uint8_t detect_right_count;
uint8_t detected_intersection;
int32_t detected_intersection_distance;
uint8_t on_dark;
int32_t on_dark_distance;
int32_t turn_goal;
int32_t state_start_millis;
int32_t state_start_distance;
int32_t follow_min_distance;

uint8_t Follow::sensors[5];
int16_t Follow::pos;
uint8_t Follow::detected_left, Follow::detected_straight, Follow::detected_right, Follow::detected_end;
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
    on_line = 0;
    
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
    
    if(!on_line)
    {
      on_line = 1;
      on_line_distance = Encoders::distance;
    }
  }
}

void Follow::update()
{
  static uint8_t last_state = STATE_NONE;
  
  readSensors();
  
  if(state != last_state)
  {
    state_start_millis = millis();
    state_start_distance = Encoders::distance;
    last_state = state;
  }
  
  switch(state)
  {
  case STATE_FOLLOWING:
    follow();
    break;
  case STATE_FOLLOWING_MORE:
    follow_more();
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
  turn_goal = angle_degrees*11;
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

void Follow::doFollow(uint32_t follow_min_d)
{ 
  follow_min_distance = follow_min_d;
  Encoders::reset();
  off_line = 0;
  detected_intersection = 0;
  detected_end = 0;
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
  
  if(millis() - state_start_millis > 100)
    state = STATE_WAITING;
}

void Follow::checkForEnd()
{   
  if(min(min(Follow::sensors[1],Follow::sensors[2]),Follow::sensors[3]) > 210)
  {
    if(!on_dark)
    {
      on_dark = 1;
      on_dark_distance = Encoders::distance;
    }
  }
  else
  {
    on_dark = 0;
  }
    
  if(on_dark && Encoders::distance - on_dark_distance > 400)
  {
    detected_end = 1;
  }
}

void Follow::check_for_intersections()
{    
  if(sensors[0] > 128)
    detect_left_count += 1;
  else
    detect_left_count = 0;
  if(detect_left_count > LEFT_RIGHT_DETECT_COUNT) // maybe add min distance
    detected_left = 1;
  if(sensors[4] > 128)
    detect_right_count += 1;
  else
    detect_right_count = 0;
  if(detect_right_count > LEFT_RIGHT_DETECT_COUNT)
    detected_right = 1;
  
  if((detected_left || detected_right) &&
    !detected_intersection)
  {
    detected_intersection = 1;
    detected_intersection_distance = Encoders::distance;
  }
  
  checkForEnd();
}

void Follow::follow_more()
{
  check_for_intersections();
  
  detected_straight = on_line && Encoders::distance - on_line_distance > 100;
    
  if(Encoders::distance - state_start_distance > 300)
  {
    state = STATE_WAITING;
    return;
  }
  
  do_pid(detected_intersection ? SPEED/2 : SPEED);
}

void Follow::follow()
{

  if(Encoders::distance - state_start_distance > follow_min_distance)
    check_for_intersections();
  
  if(off_line && Encoders::distance - off_line_distance > 300 ||
    detected_intersection && Encoders::distance - detected_intersection_distance > 300)
  {
    detected_straight = on_line && Encoders::distance - on_line_distance > 100;
    state = STATE_FOLLOWING_MORE;
  }

  uint16_t speed = SPEED;
  if(detected_intersection)
    speed = SPEED/2;
  else if(follow_min_distance - (Encoders::distance - state_start_distance) > 1500)
    speed = MAX_SPEED;
  else if(follow_min_distance - (Encoders::distance - state_start_distance) > 500)
    speed = MEDIUM_SPEED;

  do_pid(speed);
}

void Follow::do_pid(uint16_t speed)
{
  int32_t diff = last_pos - pos;
  int32_t pid = pos/4 - diff;
  
  Motors::set(limit(speed+pid,0,speed), limit(speed-pid,0,speed));
  last_pos = pos;
}
