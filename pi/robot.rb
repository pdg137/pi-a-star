#!/usr/bin/env ruby

system("echo #{Process.pid} > /var/log/web.pid")

require_relative 'a_star'

a_star = AStar.new

while true
  a_star.set_leds(true,false,false)
  sleep 0.2
  a_star.set_leds(false,false,true)
  sleep 0.2
  a_star.set_leds(false,false,false)
  sleep 0.6
end
