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
      yield turn_group[0], @unit*(turn_group.length-1) + @limit
    end
  end
end

