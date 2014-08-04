class LeftmostTurn < ResponseState::Service
  def initialize(a_star)
    @a_star = a_star
  end

  def call
    dir = nil

    @a_star.follow do |result|
      result.end {
        yield send_state(:end)
        return
      }
      result.intersection {
        # the first dir is the leftmost one
        dir = result.context[:exits].first
      }
    end

    @a_star.turn(dir) do |result|
      result.done {
        yield send_state(:turned, dir)
      }
    end
  end
end
