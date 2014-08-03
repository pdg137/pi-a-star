class AStarReport
  attr_accessor :distance, :errors1, :errors2, :buttons, :sensors
  attr_accessor :pos, :left, :straight, :right, :follow_state

  def initialize()
    @sensors = [0,0,0,0,0]
    @distance = @errors1 = @errors2 = @buttons = @pos = @left = @straight = @right = 0
  end
end
