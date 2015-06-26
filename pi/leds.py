#!/usr/bin/python

import smbus
import time
import struct

bus = smbus.SMBus(1)

def leds(red, yellow, green):
  bytes = [4, red, yellow, green]
  bus.write_i2c_block_data(20, 1, bytes)

leds(0,0,0)
time.sleep(0.3)
leds(1,0,0)
time.sleep(0.3)
leds(1,1,0)
time.sleep(0.3)
leds(0,1,1)
time.sleep(0.3)
leds(0,0,1)
time.sleep(0.3)
leds(0,0,0)
time.sleep(0.3)
leds(0,0,1)
time.sleep(0.3)
leds(0,1,1)
time.sleep(0.3)
leds(1,1,0)
time.sleep(0.3)
leds(1,0,0)
time.sleep(0.3)
leds(0,0,0)
