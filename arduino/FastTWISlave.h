#pragma once
#include <inttypes.h>

class FastTWISlave
{
public:
  /* Methods for a slave to declare */
  virtual void receive(uint8_t b) = 0;
  virtual uint8_t transmit() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;

  /* Initialize slave on a specific address; do not respond to general calls. */
  static void init(uint8_t address, FastTWISlave &slave);

  /* Low-level static methods not meant to be called by users. */
  static uint8_t handleEvent(uint8_t event);
  static void ack();
  static void nack();
  static void clearBusError();
};
