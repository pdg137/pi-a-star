require_relative 'leftmost_turn'

class MazeSolver
  def initialize(a_star)
    @a_star = a_star
  end

  def run
    @path = []

    solve_maze

    p @path

    until @a_star.get_report.button1?
      sleep(0.1)
    end

    play_back_maze
  end

  def append_turn(dir)
    @path << dir
    simplify_path
  end

  def substitute(dir)
    @path[-3..-1] = dir
  end

  def simplify_path
    # cases marked "X" should never happen
    case @path[-3..-1]
    when [:left, :back, :left] then substitute :straight
    when [:left, :back, :straight] then substitute :right
    when [:left, :back, :right] then substitute :back
    when [:left, :back, :back] then substitute :left # X

    when [:straight, :back, :left] then substitute :right
    when [:straight, :back, :straight] then substitute :back
    when [:straight, :back, :right] then substitute :left # X
    when [:straight, :back, :back] then substitute :straight # X

    when [:right, :back, :left] then substitute :back
    when [:right, :back, :straight] then substitute :left # X
    when [:right, :back, :right] then substitute :straight # X
    when [:right, :back, :back] then substitute :right # X
    end
  end

  def solve_maze
    while true
      LeftmostTurn.new(@a_star).call do |result|
        result.end { return }
        result.turned { append_turn(result.message) }
      end
    end
  end

  def play_back_maze
    @path.each do |dir|
      # TODO: check some of these response states

      @a_star.follow do |result|
        result.end { }
        result.intersection { }
      end

      @a_star.turn(dir) do |result|
        result.done { }
      end
    end
  end
end

