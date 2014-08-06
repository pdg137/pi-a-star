require 'spec_helper'

describe LoopedMazeSolver do
  context "maze 1" do
    let(:fake_a_star) do
      FakeAStar.new <<END
#   #-#-#-# #
|   |   | | |
#-# # #-#-#-#
  | | | |   |
  #-# #-#-X #
  | | |     |
#-# #-#-#-#-#
END
    end

    subject { LoopedMazeSolver.new(fake_a_star) }

    it "solves the maze" do
      fake_a_star.goto Point(0,0), Vector(1,0)

      subject.explore_to_end
      expect(fake_a_star.pos).to eq fake_a_star.maze.end
      #      expect(subject.maze).to eq fake_a_star.maze
    end
  end
end
