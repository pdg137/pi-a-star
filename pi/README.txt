* Put user into i2c and dialout groups
* You don't need to slow down the frequency, but here's how you could do it:
sudo modprobe -r i2c_bcm2708; sudo modprobe i2c_bcm2708 baudrate=10000
newer pis:
options i2c_bcm2708 baudrate=10000

I2C Protocol
============

The protocol is designed to allow the A-Star to do everything through
polling in a main loop, in a way that is tolerant of delays.  This is
especially important since the Raspberry Pi has issues with clock
stretching.

256 bytes are allocated on the A-Star for communication, with bytes
0-127 for transient commands from the master and 128-255 for
persistent data.

Byte 0 is the command status byte.  It is set to 2 (CMD_STATUS_LOCK)
in the ISR when an I2C write to the command region begins, then to 0
(CMD_STATUS_CALL) when the write stops.  The A-Star is expected to
respond to the command (filling in any return values) then set this
byte to 1 (CMD_STATUS_RETURN).

Byte 1 is the command number.

Bytes 2-127 are used for arguments and return values, in a format that
is agreed upon by the RPi and A-Star application code.

