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
  (distance, errors1, errors2, buttons,
   sensors[0],sensors[1],sensors[2],sensors[3],sensors[4],
   pos, left, right) = a_star.get_report
  if buttons & 1 == 1
    puts "Power button pressed - executing 'halt'..."
    system("halt")
  end
end
