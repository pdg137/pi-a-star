require_relative 'maze'
require_relative 'point'
require_relative 'vector'
require_relative 'gridded_maze'
require_relative 'segment_voter'
require 'set'

class LoopedMazeSolver
  # Our internal concept of the maze.
  # Updated as we explore it.
  attr_reader :maze, :pos, :vec, :explored_nodes, :a_star, :voter, :deduced_nodes

  def set_initial_position
    @pos = Point(0,0)
    @vec = Vector(1,0)
  end

  def initialize(a_star)
    @a_star = a_star
    set_initial_position
    @explored_nodes = Set.new
    @deduced_nodes = Set.new
    @maze = GriddedMaze.new
    @voter = SegmentVoter.new

    # assume we start facing a segment
    maze.connect(pos, pos+vec)
  end

  def unexplored_nodes
    maze.nodes - explored_nodes
  end

  def useful_nodes_to_explore
    # exclude the current node, and any deduced nodes if the end is known
    unexplored_nodes.reject { |node|
      pos == node || (maze.end && deduced_nodes.include?(node))
    }
  end

  def estimate_grid_units(distance)
    (distance + 3*300)/(6*300)
  end

  def connect(a,b)
    maze.connect(a,b)
    puts "connect #{a}-#{b}"
  end

  def disconnect(a,b)
    maze.disconnect(a,b)
    puts "disconnect #{a}-#{b}"
  end

  # true if all possible segments are known to either exist or not,
  # according to the voter
  def check_deductions(node)
    return if @explored_nodes.include? node

    all_neighbors_known = node.cartesian_neighbors.map do |neighbor|
      voter.known?(node, neighbor)
    end.inject(:&)

    if all_neighbors_known
      @deduced_nodes << node
    end
  end

  def observe_segment(a,b)
    voter.vote_connected(a,b)

    if voter.connected?(a,b)
      connect(a,b)
    end

    check_deductions(a)
    check_deductions(b)
  end

  def observe_no_segment(a,b)
    puts "no segment #{a}-#{b}"
    voter.vote_not_connected(a,b)
    puts "-> #{voter.known?(a,b)}"

    if voter.not_connected?(a,b) && maze.nodes.include?(a) && maze.nodes.include?(b)
      disconnect(a,b)
    end

    check_deductions(a)
    check_deductions(b)
  end

  def record_path(context)
    units = estimate_grid_units context[:distance]
    puts "distance #{context[:distance]} -> #{units} units"
    # TODO: handle zero!

    original_pos = @pos
    units.times.each do
      old_pos = pos
      @pos += vec

      puts "at #{pos} #{vec}"

      observe_segment(old_pos, pos)

      puts maze.to_s(pos)

      explored_nodes << pos
    end

    pos1 = original_pos
    left = vec.turn(:left)
    right = vec.turn(:left)
    (units-1).times.each do
      pos2 = pos1 + vec

      left_node = pos2 + left
      right_node = pos2 + right
      observe_no_segment(pos2, left_node)
      observe_no_segment(pos2, right_node)

      pos1 = pos2
    end
  end

  def record_intersection(context)
    puts "exits: #{context[:exits]}"
    [:left,:right,:straight].each do |dir|
      exit_vec = vec.turn(dir)
      if(context[:exits].include? dir)
        observe_segment(pos, pos+exit_vec)
      else
        observe_no_segment(pos, pos+exit_vec)
      end
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

      closest_unexplored_node = useful_nodes_to_explore.min_by { |node|
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
