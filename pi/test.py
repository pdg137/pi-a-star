#!/usr/bin/python

import smbus
import time

bus = smbus.SMBus(1)

i=0
while True:
  i += 1
  print(i)
  bus.write_byte(20, 1)
  bus.read_byte(20)
