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
end
