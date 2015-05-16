#include "RPiI2C.h"
#include "i2c_interface.h"

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

}

