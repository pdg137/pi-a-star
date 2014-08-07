class AStarReport
  attr_accessor :distance, :errors1, :errors2, :buttons, :sensors
  attr_accessor :pos, :left, :straight, :right, :end, :follow_state
  attr_accessor :command_count

  def initialize()
    @sensors = [0,0,0,0,0]
    @distance = @errors1 = @errors2 = @buttons = @pos = @left = @straight = @right = @end = 0
    @command_count = 0
  end

  # returns available exits in L S R B order
  def exits
    exits = []
    exits << :left if left == 1
    exits << :straight if straight == 1
    exits << :right if right == 1
    exits << :back
  end

  def button0?
    (buttons & 1) != 0
  end

  def button1?
    (buttons & 2) != 0
  end

  def button2?
    (buttons & 4) != 0
  end
end
