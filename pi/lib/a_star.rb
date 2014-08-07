require 'i2c'
require_relative 'a_star_report'
require_relative 'follow'
require_relative 'turn'

class AStar
  attr_reader :command_count

  def initialize
    @i2c = I2C.create('/dev/i2c-1')
    @command_count = 0
  end

  def send_command_data(command, follow_min_distance)
    @i2c.write(20, [0,@command_count,command, follow_min_distance].pack("CCCL"))
  end

  def send_command(command, follow_min_distance)
    # For some reason, when we send 128, it gets mapped to zero
    # and causes an infinite loop.
    @command_count = (@command_count + 1) % 16
    send_command_data command, follow_min_distance
    start = Time.now

    while get_report.command_count != command_count
      sleep(0.01)
      if 1000*(Time.now - start) > 20
        send_command_data command
        start = Time.now
      end
    end
  end

  def turn_left
    send_command(1,0)
  end

  def turn_right
    send_command(2,0)
  end

  def turn_back
    send_command(3,0)
  end

  def send_follow_command
    send_command(4,600)
  end

  def get_raw_report
    return @i2c.read(20,26).unpack("ClLLCCCCCCsccccc")
  end

  def get_report
    report = AStarReport.new
    (report.command_count,
     report.distance, report.errors1, report.errors2, report.buttons,
     report.sensors[0], report.sensors[1], report.sensors[2],
     report.sensors[3], report.sensors[4],
     report.pos, report.follow_state,
     report.left, report.straight, report.right,
     report.end) = get_raw_report
    report
  end

  def set_leds(red,yellow,green)
    led = (red ? 1 : 0) +
      (yellow ? 2 : 0 ) +
      (green ? 4 : 0)
    @i2c.write(20, [6,1,led].pack("CCC"))
  end

  def follow(follow_min_distance, &block)
    return Follow.new(self).call(&block)
  end

  def turn(dir, &block)
    Turn.new(self, dir).call(&block)
  end
end
