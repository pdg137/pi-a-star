#!/usr/bin/ruby

require 'i2c'

bus = I2C.create('/dev/i2c-1')

bus.write(20, 200, "abcd")
bus.write(20, 201, "bcde")
while true
  raise if "abcde" != bus.read(20, 5, 200)
end
