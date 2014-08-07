INF = Float::INFINITY

class Maze
  attr_reader :nodes, :connections
  def initialize(nodes = [], connections = {})
    @nodes = nodes.to_set
    @connections = connections
    connections.default = [].to_set
  end

  def add_node(node)
    nodes.add node
  end

  def add_connections(params)
    params.each_pair do |node, neighbors|
      # avoid problem with duplicates
      if connections[node].empty?
        connections[node] = [].to_set
      end
      connections[node].merge neighbors
    end
  end

  def remove_connections(params)
    params.each_pair do |node, neighbors|
      connections[node].subtract neighbors
    end
  end

  def connect(node1, node2)
    add_node node1
    add_node node2
    add_connections node1 => [node2], node2 => [node1]
  end

  def disconnect(node1, node2)
    add_node node1
    add_node node2
    remove_connections node1 => [node2], node2 => [node1]
  end

  def solve(goal)
    score = initial_score(goal)
    unchecked_nodes = nodes.dup # because we are using delete

    until unchecked_nodes.empty?
      node = closest(unchecked_nodes, score)
      check(score, node)
      unchecked_nodes.delete node
    end

    score
  end

  def initial_score(goal)
    score = {}
    score.default = INF
    score[goal] = 0
    score
  end

  def closest(nodes, scores)
    nodes.min_by { |x| scores[x] }
  end

  def check(score, node)
    connections[node].each do |neighbor|
      score[neighbor] = score[node]+1 if score[neighbor] > score[node]
    end
  end

  def get_path(from_node, to_node)
    score = solve(to_node)

    raise "node #{from_node.inspect} not present" if !nodes.include? from_node
    raise "node #{to_node.inspect} not present" if !nodes.include? to_node
    return nil if score[from_node] == INF

    path = [from_node]
    while path.last != to_node
      neighbors = connections[path.last]
      next_node = neighbors.min_by { |x| score[x] }
      return nil if next_node.nil?
      path << next_node
    end

    path
  end
end
