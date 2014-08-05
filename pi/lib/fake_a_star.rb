require 'response_state'

class FakeAStar
  attr_reader :maze, :pos, :vec

  def initialize(map)
    @maze = GriddedMaze.from_s map
  end

  def goto(point, vector)
    @pos = point
    @vec = vector
  end

  def turn(dir)
    response = ResponseState::Response.new(:done)
    @vec = @vec.turn dir
    yield response
  end

  def follow
    @pos += @vec

    # get exits sorted in leftmost order
    exits = @maze.connections[@pos].map do |neighbor|
      @vec.dir_to(neighbor - @pos)
    end.sort_by do |dir|
      [:left,:straight,:right,:back].index dir
    end

    response = ResponseState::Response.new(:intersection,
                                           "",
                                           {exits: exits}
                                           )
    yield response
  end
end
