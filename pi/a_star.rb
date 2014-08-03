require 'i2c'

class AStar
  def initialize
    @i2c = I2C.create('/dev/i2c-1')
  end

  def send_command(command)
    @i2c.write(20, [0,1,command].pack("CCC"))
  end

  def turn_left
    send_command(1)
  end

  def turn_right
    send_command(2)
  end

  def turn_back
    send_command(3)
  end

  def follow
    send_command(4)
  end

  def get_report
    return @i2c.read(20,23).unpack("lLLCCCCCCsccc")
  end

  def set_leds(red,yellow,green)
    led = (red ? 1 : 0) +
      (yellow ? 2 : 0 ) +
      (green ? 4 : 0)
    @i2c.write(20, [2,1,led].pack("CCC"))
  end
end
