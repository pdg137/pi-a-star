import smbus
import struct

class AStar(object):
  def __init__(self):
    self.bus = smbus.SMBus(1)

  def read_unpack(self, address, size, format):
    self.bus.write_byte(20,address)
    byte_list = []
    for n in range(0,size):
      byte_list.append(self.bus.read_byte(20))
    return struct.unpack(format,bytes(bytearray(byte_list)))

  def leds(self, red, yellow, green):
    self.bus.write_i2c_block_data(20, 0, [red, yellow, green])

  def motors(self, left, right):
    self.bus.write_i2c_block_data(20, 6, map(ord, list(struct.pack('hh', left, right))))

  def read_buttons(self):
    return self.read_unpack(3, 3, "???")

  def read_battery_millivolts(self):
    return self.read_unpack(10, 2, "H")

  def read_analog(self):
    return self.read_unpack(12, 12, "HHHHHH")
