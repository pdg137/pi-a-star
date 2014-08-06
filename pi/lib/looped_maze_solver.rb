require_relative 'maze'
require_relative 'point'
require_relative 'vector'
require_relative 'gridded_maze'
require 'set'

class LoopedMazeSolver
  # Our internal concept of the maze.
  # Updated as we explore it.
  attr_reader :maze, :pos, :vec, :explored_nodes, :a_star

  def initialize(a_star)
    @a_star = a_star
    @pos = Point(0,0)
    @vec = Vector(1,0)
    @explored_nodes = Set.new
    @explored_nodes << pos
    @maze = GriddedMaze.new

    # assume we start facing a segment
    maze.connect(pos, pos+vec)
  end

  def unexplored_nodes
    maze.nodes - explored_nodes
  end

  def estimate_grid_units(distance)
    (distance + 3*280)/(6*280)
  end

  def connect(a,b)
    maze.connect(a,b)
    puts "connect #{a}-#{b}"
  end

  def record_intersection(context)
    units = estimate_grid_units context[:distance]
    puts "distance #{context[:distance]} -> #{units} units"
    # TODO: handle zero!

    units.times.each do
      old_pos = pos
      @pos += vec
      puts "at #{pos} #{vec}"

      connect(old_pos, pos)

      explored_nodes << pos
    end

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
      end

      puts "follow"

      a_star.follow do |result|
        result.end {
          record_intersection(result.context)
          maze.end = pos
        }
        result.intersection {
          record_intersection(result.context)
        }
      end

    end
  end

  def explore_to_end
    while pos != maze.end
      distances_from_pos = maze.solve(pos)

      closest_unexplored_node = unexplored_nodes.min_by { |node|
        distances_from_pos[node]
      }

      explore_to closest_unexplored_node
    end
  end
end

