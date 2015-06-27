require 'i2c'

class AStar
  def initialize
    @i2c = I2C.create('/dev/i2c-1')
  end

  def write
    @i2c.write(20, 1, 1, 'FrogsFrogsFrogsFrogs')
  end

  def set_motors(left, right)
    @i2c.write(20, 1, 2, [left, right].pack("ss"))
    wait_for_return
  end

  def read
    @i2c.read(20, 20, 2)
  end

  def wait_for_return
    while 0 == @i2c.read(20, 1) # <-- wrong?
    end
  end
end
