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

  (count1, count2, errors1, errors2, buttons) = a_star.get_report
  if buttons & 1 == 1
    puts "Power button pressed - executing 'halt'..."
    system("halt")
  end
end
