require 'i2c'

class AStar
  def initialize
    @i2c = I2C.create('/dev/i2c-1')
  end

  def set_motors(m1, m2)
    @i2c.write(20, [0,1,m1,m2].pack("CCss"))
  end

  def get_report
    return @i2c.read(20,10).unpack("ccLL")
  end
end
