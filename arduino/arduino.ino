extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  #include "rpi_twi.h"
}

#define COMMAND_NONE 0
#define COMMAND_TURN_LEFT 1
#define COMMAND_TURN_RIGHT 2
#define COMMAND_TURN_BACK 3
#define COMMAND_FOLLOW 4

void setup()
{
  pinMode(13, OUTPUT);
  twi_init();
  twi_setAddress(20);
}

void loop()
{

}

