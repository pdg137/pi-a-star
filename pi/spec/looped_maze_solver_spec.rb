require 'spec_helper'

describe LoopedMazeSolver do
  subject { LoopedMazeSolver.new(fake_a_star) }

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

    it "solves the maze" do
      fake_a_star.goto Point(0,0), Vector(1,0)

      subject.explore_to_end
      expect(fake_a_star.pos).to eq fake_a_star.maze.end

      subject.explore_entire_maze
      expect(fake_a_star.maze.nodes).to eq subject.maze.nodes

      fake_a_star.goto Point(0,0), Vector(1,0)
      subject.replay_from_zero
      expect(fake_a_star.pos).to eq fake_a_star.maze.end
    end

    it "solves the maze starting from a dead end" do
      fake_a_star.goto Point(1,0), Vector(-1,0)

      subject.explore_to_end
      expect(fake_a_star.pos).to eq fake_a_star.maze.end
    end
  end

  context "easily optimizable maze" do
    let(:fake_a_star) do
      FakeAStar.new <<END
#-#-#
|   |
#-# #
    |
X-#-#
END
    end

    before do
      fake_a_star.goto Point(1,0), Vector(1,0)
    end

    it "solves without exploring the central node (0,1)" do
      subject.explore_to_end
      expect(fake_a_star.pos).to eq fake_a_star.maze.end
      subject.explore_entire_maze
      expect(subject.explored_nodes).to_not include Point(0,1)
    end
  end

  context "non-optimizable maze" do
    let(:fake_a_star) do
      FakeAStar.new <<END
#-#-#
|   |
#-X #
|   |
# #-#
END
    end

    before do
      fake_a_star.goto Point(1,0), Vector(1,0)
    end

    it "solves the maze" do
      subject.explore_to_end
      expect(fake_a_star.pos).to eq fake_a_star.maze.end
    end
  end
end
