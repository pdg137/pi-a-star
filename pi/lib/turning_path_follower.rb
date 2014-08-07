class TurningPathFollower
  def initialize(unit, limit)
    @unit = unit
    @limit = limit
  end

  def compute(turning_path)
    turning_path.slice_before { |turn|
      [:left,:right,:back].include? turn
    }.
      each do |turn_group|

      until turn_group.nil?
        current = turn_group[0..5]
        turn_group = turn_group[6..-1]

        # e.g. [:straight, :straight] [:none, :none]
        # -> [:straight], [:straight, :none, :none]
        while turn_group != nil && turn_group[0] == :none
          turn_group.unshift current.pop
        end

        yield current[0], @unit*(current.length-1) + @limit
      end
    end
  end
end

