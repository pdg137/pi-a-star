require 'i2c'

class AStar
  def initialize
    @i2c = I2C.create('/dev/i2c-1')
  end

  def write
    @i2c.write(20, 1, 1, 'FrogsFrogsFrogsFrogs')
  end

  def read
    @i2c.read(20, 20, 2)
  end

  def wait_for_return
    while 0 == @i2c.read(20, 1)
    end
  end
end
