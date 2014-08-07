class SegmentVoter
  def initialize
    @votes = {}
  end

  def [](a,b)
    @votes[a] ||= {}
    @votes[a][b] ||= 0
  end

  def []=(a,b,x)
    @votes[a] ||= {}
    @votes[a][b] = x

    @votes[b] ||= {}
    @votes[b][a] = x
  end

  def vote_connected(a, b)
    self[a,b] += 1
  end

  def vote_not_connected(a, b)
    self[a,b] -= 1
  end

  def connected?(a, b)
    case
    when self[a,b] < 0 then false
    when self[a,b] > 0 then true
    else nil
    end
  end

  def not_connected?(a, b)
    # nil is unknown
    connected?(a, b) == false
  end

  def known?(a, b)
    connected?(a, b) != nil
  end
end
