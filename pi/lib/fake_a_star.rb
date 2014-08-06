require 'response_state'

class FakeAStar
  class OffLineException < Exception
  end

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
    original_pos = @pos
    exits = nil
    while !exits || exits == [:straight, :back]
      new_pos = @pos + @vec

      if !@maze.connections[@pos].include? new_pos
        raise OffLineException.new
      end

      @pos = new_pos

      # get exits sorted in leftmost order
      exits = @maze.connections[@pos].map do |neighbor|
        @vec.dir_to(neighbor - @pos)
      end.sort_by do |dir|
        [:left,:straight,:right,:back].index dir
      end
    end

    status = if maze.end == @pos
               :end
             else
               :intersection
             end

    distance = (@pos - original_pos).length * 280 * 6 + (rand(60) - 30)

    response = ResponseState::Response.new(status,
                                           "",
                                           {exits: exits, distance: distance}
                                           )
    yield response
  end
end
