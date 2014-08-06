require_relative 'maze'
require_relative 'point'
require_relative 'vector'
require_relative 'gridded_maze'
require 'set'

class LoopedMazeSolver
  # Our internal concept of the maze.
  # Updated as we explore it.
  attr_reader :maze, :pos, :vec, :explored_nodes, :a_star

  def set_initial_position
    @pos = Point(0,0)
    @vec = Vector(1,0)
  end

  def initialize(a_star)
    @a_star = a_star
    set_initial_position
    @explored_nodes = Set.new
    @maze = GriddedMaze.new

    # assume we start facing a segment
    maze.connect(pos, pos+vec)
  end

  def unexplored_nodes
    maze.nodes - explored_nodes
  end

  def unexplored_other_nodes
    unexplored_nodes.reject { |node| pos == node }
  end

  def estimate_grid_units(distance)
    (distance + 3*300)/(6*300)
  end

  def connect(a,b)
    maze.connect(a,b)
    puts "connect #{a}-#{b}"
  end

  def record_path(context)
    units = estimate_grid_units context[:distance]
    puts "distance #{context[:distance]} -> #{units} units"
    # TODO: handle zero!

    units.times.each do
      old_pos = pos
      @pos += vec

      puts "at #{pos} #{vec}"

      connect(old_pos, pos)

      puts maze.to_s(pos)

      explored_nodes << pos
    end
  end

  def record_intersection(context)
    puts "exits: #{context[:exits]}"
    context[:exits].each do |dir|
      next if dir == :back # this is already covered
      exit_vec = vec.turn(dir)
      connect(pos, pos+exit_vec)
    end
  end

  def explore_to(target)
    puts "\nexplore to #{target}"

    maze.get_turning_path(vec, pos, target).each do |turn|
      puts turn
      a_star.turn(turn) do |result|
        result.done {
          @vec = vec.turn(turn)
        }
        result.button { raise }
      end

      puts "follow"

      a_star.follow do |result|
        result.end {
          record_path(result.context)
          maze.end = pos
        }
        result.intersection {
          record_path(result.context)
          record_intersection(result.context)
        }
        result.button { raise }
      end

    end
  end

  def explore_to_end
    explore(true)
  end

  def explore_entire_maze
    explore(false)
  end

  def explore(stop_on_end)
    puts "\n\STARTING EXPLORATION FROM #{pos}\n"

    while true
      break if stop_on_end && pos == maze.end

      distances_from_pos = maze.solve(pos)

      closest_unexplored_node = unexplored_other_nodes.min_by { |node|
        distances_from_pos[node]
      }

      # TODO: handle this better
      break if closest_unexplored_node.nil?

      explore_to closest_unexplored_node
    end
  end

  def replay_from_zero
    set_initial_position
    explore_to maze.end
  end
end
