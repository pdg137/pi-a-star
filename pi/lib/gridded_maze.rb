require_relative 'point'
require_relative 'vector'

class GriddedMaze < Maze
  def add_intersection(node, exit_vectors)
    exit_vectors.each do |vector|
      unit = vector.unit
      length = vector.length

      old_node = node
      length.times.each do
        new_node = old_node+unit
        add_node new_node
        add_connections old_node => [new_node], new_node => [old_node]
        old_node = new_node
      end

    end
  end

  def add_node(node)
    if node.is_a? Point
      super node
    else
      super Point.new(node)
    end
  end

  def self.vectors_to_turn(start, finish)
    start.assert_cardinal
    finish.assert_cardinal

    a = [Vector(1,0),Vector(0,1),Vector(-1,0),Vector(0,-1)].index start
    b = [Vector(1,0),Vector(0,1),Vector(-1,0),Vector(0,-1)].index finish

    left_amount = b - a
    left_amount += 4 if left_amount < 0

    [:straight, :left, :back, :right][left_amount]
  end

  def get_turning_path(vector, start, finish)
    vector.assert_cardinal

    path = get_path(start, finish)
    turns = []

    last_node = path.shift
    current_node = path.shift
    last_dir = current_node - last_node

    initial_turn = GriddedMaze.vectors_to_turn(vector, last_dir)

    until path.empty?
      next_node = path.shift
      next_dir = next_node - current_node

      # skip straights on nodes with no other neighbors since the
      # robot will drive right through them without seeing a node
      if !(connections[current_node].length == 2 && last_dir == next_dir)
        turns << GriddedMaze.vectors_to_turn(last_dir, next_dir)
      end

      last_node = current_node
      current_node = next_node
      last_dir = next_dir
    end

    {turns: turns, initial_turn: initial_turn}
  end

  def self.from_s(string)
    maze = GriddedMaze.new

    lines = string.lines.map &:rstrip
    raise "need odd number of lines" if lines.length % 2 != 1

    # the last line should be y-coordinate 0
    lines.reverse!

    lines.each_index do |map_y|
      line = lines[map_y]
      line.length.times do |map_x|
        odd_y = (map_y % 2 == 1)
        odd_x = (map_x % 2 == 1)
        char = line[map_x]

        # skip blanks
        blank = (char == " ")
        next if blank

        raise "non-blank char #{char} at #{map_x}, #{map_y}" if odd_x && odd_y

        if !odd_x && !odd_y
          # a node
          maze.add_node(Point map_x/2, map_y/2)
        elsif odd_x
          # east/west connection
          maze.connect(Point((map_x-1)/2,map_y/2),
                       Point((map_x+1)/2,map_y/2))
        else
          # north/south connection
          maze.connect(Point(map_x/2,(map_y-1)/2),
                       Point(map_x/2,(map_y+1)/2))
        end
      end
    end

    maze
  end
end
