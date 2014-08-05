class Maze
  attr_reader :nodes, :connections
  def initialize(nodes = [], connections = {})
    @nodes = nodes
    @connections = connections
    connections.default = []
  end

  def add_node(node)
    return if index(node)
    nodes << node
  end

  def add_connections(params)
    params.each_pair do |node, neighbors|
      connections[node] |= neighbors
    end
  end

  def solve(goal)
    score = initial_score(goal)
    unchecked_nodes = nodes

    until unchecked_nodes.empty?
      node = closest(unchecked_nodes, score)
      check(score, node)
      unchecked_nodes -= [node]
    end

    score
  end

  def index(node)
    nodes.index node
  end

  def initial_score(goal)
    score = [INF]*nodes.length
    score[index goal] = 0
    score
  end

  def closest(nodes, scores)
    nodes.min_by { |x| scores[index x] }
  end

  def check(score, node)
    connections[node].each do |neighbor|
      score[index neighbor] = score[index node]+1 if score[index neighbor] > score[index node]
    end
  end

  def path(from_node, to_node)
    score = solve(to_node)

    return nil if score[index from_node] == INF

    path = [from_node]
    while path.last != to_node
      neighbors = connections[path.last]
      next_node = neighbors.min_by { |x| score[index x] }
      return nil if next_node.nil?
      path << next_node
    end

    path
  end
end
