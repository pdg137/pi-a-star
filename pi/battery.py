#!/usr/bin/python

import smbus
import time
import struct

bus = smbus.SMBus(1)

def command_done():
    bus.write_byte(20,0)
    return bus.read_byte(20) == 0

def read():
    bus.write_i2c_block_data(20, 1, [1])
    while(not command_done()):
        pass
    bus.write_byte(20,2)
    byte_list = []
    for n in range(0,2):
        byte_list.append(bus.read_byte(20))
    return struct.unpack("H",bytes(bytearray(byte_list)))[0]

print(read())
