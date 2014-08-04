require_relative 'leftmost_turn'

class MazeSolver
  def self.run(a_star)
    @path = []

    while true
      return if a_star.get_report.button2?

      LeftmostTurn.new(a_star).call do |result|
        result.end { return }
        result.turned {  }
      end
    end
  end
end

