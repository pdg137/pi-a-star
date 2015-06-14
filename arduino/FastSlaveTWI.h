#pragma once
#include <inttypes.h>

class FastSlaveTWI
{
public:

  class Slave {
  public:
    virtual void receive(uint8_t b) = 0;
    virtual uint8_t transmit() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
  };

  /* Initialize slave on a specific address; do not respond to general calls. */
  static void init(uint8_t address, Slave &slave);
  static uint8_t handle_event(uint8_t event);
  static uint8_t getByte(uint8_t index);
  static void setByte(uint8_t index, uint8_t value);
  
  /* Check for a command and return the command # if we are being called. */
  static uint8_t checkForCommand();
  
  /* Indicate that we are done processing a command. */
  static void commandReturn();

  /* These are used internally */
  static void ack();
  static void nack();
};
