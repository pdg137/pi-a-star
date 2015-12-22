#pragma once
#include "FastTWISlave.h"

/* RPiSlave is an extension of FastTWISlave that (ironically) slows
 * down communication where necessary to work around the RPi I2C clock
 * stretching bug.  Additionally, it implements a system of buffers
 * allowing user code and the I2C system to read and write
 * asynchronously from the same data, without dictating any particular
 * protocol.
 *
 * The buffer size is set to 32 bytes, but this can be changed as
 * necessary by editing BUFFER_LENGTH below.  As described below, we
 * use FIVE copies of the buffer, so watch your RAM if you plan on
 * expanding it.
 *
 * You probably don't have to worry about the details below, since the
 * point of this buffering is to make it simple: all you need to do is
 * call updateBuffer() before using the buffer, do your writes and
 * reads, then call finalizeWrites() when you are done.  The I2C
 * master can read and write to the same data at any time, and you
 * should never encounter inconsistent data unless both sides attempt
 * to write to the same region simultaneously.
 *
 * Buffering details:
 *
 * The point is that since reads and writes involving I2C and user
 * code are asynchronous and slow, but we want these slow operations
 * to be effectively atomic, each side needs its own buffer that only
 * gets synchronized with the other at designated times.
 * 
 * We use two additional buffers to keep track of the original values
 * as of the last sync time, so that we know what bytes were written
 * and need to be copied.  Without these additional arrays,
 * bidirectional simultaneous writes would not be possible.
 * 
 * The fifth and final copy of the buffer keeps track of the value
 * after the last write was finalized.  It will always be equal to one
 * of the two original-value buffers, so we could probably do away
 * with it, but that would increase the complexity of the code.
 */

const uint8_t BUFFER_LENGTH = 32;

class RPiSlave: public FastTWISlave
{
private:
  char i2c_buffer[BUFFER_LENGTH];
  char i2c_buffer_old[BUFFER_LENGTH];

  char staging_buffer[BUFFER_LENGTH];
  char buffer_old[BUFFER_LENGTH];

  unsigned char index;
  unsigned char index_set = 0;

  void piDelay();

  void updateI2CBuffer();
  void finalizeI2CWrites();

public:  

  char buffer[BUFFER_LENGTH];

  void updateBuffer();
  void finalizeWrites();

  virtual void receive(uint8_t b);
  virtual uint8_t transmit();
  virtual void start();
  virtual void stop();

  /* Initialize the slave on a given address. */
  void init(unsigned char address);
};
