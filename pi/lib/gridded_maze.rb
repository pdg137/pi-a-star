class GriddedMaze < Maze
  def self.assert_cardinal(vector)
    raise "only works with cardinal directions, but given #{vector}" if (vector[0] != 0 && vector[1] != 0) || (vector[0] == 0 && vector[1] == 0)
  end

  def self.unit(vector)
    self.assert_cardinal(vector)
    if vector[0] > 0
      [1,0]
    elsif vector[1] > 0
      [0,1]
    elsif vector[0] < 0
      [-1,0]
    else
      [0,-1]
    end
  end

  def self.length(vector)
    self.assert_cardinal(vector)
    if vector[0] != 0
      vector[0].abs
    else
      vector[1].abs
    end
  end

  def self.subtract(a, b)
    [a[0]-b[0],a[1]-b[1]]
  end

  def add_intersection(node, exit_vectors)
    exit_vectors.each do |vector|
      unit = GriddedMaze.unit(vector)
      length = GriddedMaze.length(vector)

      old_node = node
      length.times.each do
        new_node = [old_node[0]+unit[0],old_node[1]+unit[1]]
        add_node new_node
        add_connections old_node => [new_node], new_node => [old_node]
        old_node = new_node
      end

    end
  end

  def add_node(node)
    node.freeze
    super node
  end

  def self.vectors_to_turn(start, finish)
    assert_cardinal start
    assert_cardinal finish

    a = [[1,0],[0,1],[-1,0],[0,-1]].index start
    b = [[1,0],[0,1],[-1,0],[0,-1]].index finish

    left_amount = b - a
    left_amount += 4 if left_amount < 0

    [:straight, :left, :back, :right][left_amount]
  end

  def get_turning_path(vector, start, finish)
    GriddedMaze.assert_cardinal vector

    path = get_path(start, finish)
    turns = []

    # TODO: initial turn

    last_node = path.shift
    current_node = path.shift
    last_dir = GriddedMaze.subtract(current_node, last_node)

    initial_turn = GriddedMaze.vectors_to_turn(vector, last_dir)

    until path.empty?
      next_node = path.shift
      next_dir = GriddedMaze.subtract(next_node, current_node)

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
end
