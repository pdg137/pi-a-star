#include "RPiI2C.h"
#include "i2c_interface.h"
#include "motors.h"
#include "encoders.h"
#include "follow.h"
#include "buttons.h"

#define COMMAND_NONE 0
#define COMMAND_TURN_LEFT 1
#define COMMAND_TURN_RIGHT 2
#define COMMAND_TURN_BACK 3
#define COMMAND_FOLLOW 4

uint8_t input[15];
uint8_t output[15];

void setup()
{
  pinMode(13, OUTPUT);
  
  I2cInterface::setup(20, input, sizeof(input),
    output, sizeof(output));
}

void check_command()
{
  uint8_t new_command=COMMAND_NONE;
  
  cli();
  // do some sensitive stuff here
  sei();
}

void loop()
{
  if(input[0] & 1) RXLED1;
  else RXLED0;
  
//  if(input[0] & 2) digitalWrite(13, HIGH);
//  else digitalWrite(13, LOW);
  
  if(input[0] & 4) TXLED1;
  else TXLED0;
  
  check_command();
}

