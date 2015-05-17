require 'i2c'
require_relative 'a_star_report'
require_relative 'follow'
require_relative 'turn'

class AStar
  def initialize
    @i2c = I2C.create('/dev/i2c-1')
  end

  def set_leds(red,yellow,green)
    led = (red ? 1 : 0) +
      (yellow ? 2 : 0 ) +
      (green ? 4 : 0)
    @i2c.write(20, [led].pack("C"))
  end

  def read
    @i2c.read(20, 5)
  end
end
