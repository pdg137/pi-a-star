#!/usr/bin/env ruby

$stdout.sync = $stderr.sync = true

require_relative 'lib/a_star'

a_star = AStar.new

400.times do |x|
  a_star.set_motors(x,x)
end
sleep 0.5
400.times do |x|
  a_star.set_motors(400-x,400-x)
end

400.times do |x|
  a_star.set_motors(-x,-x)
end
sleep 0.5
400.times do |x|
  a_star.set_motors(-400+x,-400+x)
end
