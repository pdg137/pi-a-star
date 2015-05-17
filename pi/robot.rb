#!/usr/bin/env ruby

$stdout.sync = $stderr.sync = true

require_relative 'lib/a_star'

a_star = AStar.new

t=0
while true
  100.times do
    a_star.write
    a_star.wait_for_return
    a_star.read
  end

  puts 'here'
end
