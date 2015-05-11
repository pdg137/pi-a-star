#!/usr/bin/env ruby

$stdout.sync = $stderr.sync = true

require_relative 'lib/a_star'
require_relative 'lib/looped_maze_solver'

a_star = AStar.new

while true
  a_star.set_leds(true,false,false)
  sleep 0.2
  a_star.set_leds(false,false,true)
  sleep 0.2
  a_star.set_leds(false,false,false)
  sleep 0.6

#  report = a_star.get_report
end
