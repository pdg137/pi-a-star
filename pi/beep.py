#!/usr/bin/python

import smbus
import time
import struct

bus = smbus.SMBus(1)

def play(notes):
    bus.write_i2c_block_data(20, 1, [3]+map(ord, notes)+[0])

play("v10l16cg>cgc")
