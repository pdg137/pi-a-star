require 'i2c'

class AStar
  def initialize
    @i2c = I2C.create('/dev/i2c-1')
  end

  def set_motors(m1, m2)
    @i2c.write(20, [0,1,m1,m2].pack("CCss"))
  end

  def get_report
    return @i2c.read(20,16).unpack("ccLLCCCCCC")
  end

  def set_leds(red,yellow,green)
    led = (red ? 1 : 0) +
      (yellow ? 2 : 0 ) +
      (green ? 4 : 0)
    @i2c.write(20, [5,1,led].pack("CCC"))
  end
end
