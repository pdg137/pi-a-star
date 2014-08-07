#!/usr/bin/env ruby

system("echo #{Process.pid} > /var/run/robot.pid")
$stdout.sync = $stderr.sync = true

require_relative 'lib/a_star'
require_relative 'lib/looped_maze_solver'

a_star = AStar.new

def do_maze_solving(a_star)
  solver = LoopedMazeSolver.new(a_star)
  a_star.set_leds(false, false, true)
  sleep(0.5)
  a_star.set_leds(false, false, false)
  sleep(0.5)

  solver.explore_to_end
  if solver.maze.end
    a_star.set_leds(true, true, true)
  else
    a_star.set_leds(true, false, false)
  end
  sleep(1)
  a_star.set_leds(false, false, false)

  solver.explore_entire_maze

  while true
    report = a_star.get_report

    if report.button2?
      a_star.set_leds(true, false, false)
      sleep(1)
      if a_star.get_report.button2?
        break
      end
    end

    if report.button1?
      a_star.set_leds(true, true, true)
      sleep(0.5)
      a_star.set_leds(false, false, false)
      sleep(0.5)
      solver.replay_from_zero
    end

    if solver.maze.end
      a_star.set_leds(true, true, true)
    else
      a_star.set_leds(true, false, false)
    end
    sleep(0.1)
    a_star.set_leds(false,false,false)
    sleep(0.1)
  end
end

while true
  a_star.set_leds(true,false,false)
  sleep 0.2
  a_star.set_leds(false,false,true)
  sleep 0.2
  a_star.set_leds(false,false,false)
  sleep 0.6

  sensors = [0,0,0,0,0]
  report = a_star.get_report
  if report.button0?
    puts "Power button pressed - executing 'halt'..."
    system("halt")
  end

  if report.button1?
    do_maze_solving(a_star)
  end
end
