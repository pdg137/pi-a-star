#!/usr/bin/python

import smbus
import time
import struct

bus = smbus.SMBus(1)

def motors(left, right):
  bytes = [2] + map(ord, list(struct.pack('hh', left, right)))
  bus.write_i2c_block_data(20, 1, bytes)

motors(400,-400)
time.sleep(1)

motors(399,399)
time.sleep(4)
motors(0,0)
