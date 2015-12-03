This is an example project that shows how to do communication between an [A-Star 32U4](https://www.pololu.com/category/149/a-star-programmable-controllers) and a
Raspberry Pi over I2C.  While I developed it before the [A-Star Robot Controller](https://www.pololu.com/product/3117) was released,
it is compatible with that controller.

Background
----------

I2C is a protocol for sending packets of bytes back and forth between a master and slave; it is left up to the
user to decide what the bytes in each packet mean.  In typical applications, I2C is used to access a shared
region of memory, with the first byte of the packet representing an address and additional bytes being read or
written starting at that location.  Since writes and reads are controlled by the master, the slave must be
ready to handle them at any time, which makes it a challenge to handle multi-byte data safely.  Other challenges:

* There is no standard procedure for issuing a "command" rather than just updating some data.
* The processor on the Raspberry Pi has [a bug](http://www.advamation.com/knowhow/raspberrypi/rpi-i2c-bug.html)
  that makes reliable I2C hard to establish except at very low speeds.
* The Arduino Wire library, which is the standard for I2C on the Arduino, is inefficient and does not allow the
  required workarounds for the I2C bug mentioned above.
