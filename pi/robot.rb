#!/usr/bin/env ruby

system("echo #{Process.pid} > /var/run/robot.pid")
$stdout.sync = $stderr.sync = true

require_relative 'a_star'

a_star = AStar.new

while true
  a_star.set_leds(true,false,false)
  sleep 0.2
  a_star.set_leds(false,false,true)
  sleep 0.2
  a_star.set_leds(false,false,false)
  sleep 0.6

  sensors = [0,0,0,0,0]
  report = a_star.get_report
  if report.buttons & 1 == 1
    puts "Power button pressed - executing 'halt'..."
    system("halt")
  end

  if report.buttons & 2 != 0
    while true
      if (a_star.get_report.buttons & 4) != 0
        break
      end

      a_star.follow
      sleep(0.1) # let it start

      until a_star.get_report.follow_state == 0
        sleep(0.1)
      end

      report = a_star.get_report

      turn = :back
      turn = :right if (a_star.get_report.right == 1)
      turn = :straight if (a_star.get_report.straight == 1)
      turn = :left if (a_star.get_report.left == 1)

      puts turn

      case turn
      when :back
        a_star.turn_back
      when :right
        a_star.turn_right
      when :left
        a_star.turn_left
      else
        # do nothing for straight
      end

      sleep(0.1)

      until a_star.get_report.follow_state == 0
        sleep(0.1)
      end
    end
  end
end
