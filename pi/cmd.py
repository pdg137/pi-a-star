#!/usr/bin/python

import smbus
import time
import struct

bus = smbus.SMBus(1)

def read():
  byte_list = []
  bus.write_byte(20, 2+32+2)
  for n in range(0,31):
    byte_list.append(bus.read_byte(20))
  return bytearray(byte_list).decode("utf-8")

def check_command():
  bus.write_byte(20, 2+32)
  status = bus.read_byte(20)
  command = bus.read_byte(20)
  if status == 1:
    return command
  return 0

def command_return():
  bus.write_byte_data(20,2+32,0)

while True:
  if check_command():
    print(read())
    command_return()
  time.sleep(0.1)
