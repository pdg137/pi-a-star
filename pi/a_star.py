import smbus
import struct

class AStar(object):
  def __init__(self):
    self.bus = smbus.SMBus(1)

  def command(self, cmd, byte_array):
    bytes = [cmd] + byte_array
    self.bus.write_i2c_block_data(20, 1, bytes)
    while(not self.command_done()):
      pass

  def command_done(self):
    self.bus.write_byte(20,0)
    return self.bus.read_byte(20) == 0

  def read_unpack(self, format):
    self.bus.write_byte(20,2)
    byte_list = []
    for n in range(0,2):
      byte_list.append(self.bus.read_byte(20))
    return struct.unpack(format,bytes(bytearray(byte_list)))

  def motors(self, left, right):
    self.command(2, map(ord, list(struct.pack('hh', left, right))))

  def leds(self, red, yellow, green):
    self.command(4, [red, yellow, green])

  def play(self, notes):
    self.command(3, map(ord, notes)+[0])

  def analog_read(self, port):
    self.command(5, [port])
    return self.read_unpack("H")[0]

  def read_battery_millivolts(self):
    self.command(1, [])
    return self.read_unpack("H")[0]
