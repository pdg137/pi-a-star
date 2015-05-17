#!/usr/bin/env ruby

$stdout.sync = $stderr.sync = true

require_relative 'lib/a_star'

a_star = AStar.new

t=0
while true
  delay = (Math.cos(t/2.0)*0.1).abs

  a_star.set_leds(true,false,false)
  sleep delay
  a_star.set_leds(false,false,true)
  sleep delay
  a_star.set_leds(false,true,false)
  sleep delay

  puts a_star.read.inspect

  t += 0.1 + delay

#  report = a_star.get_report
end
