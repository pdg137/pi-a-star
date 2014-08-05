require 'spec_helper'

describe "GriddedMaze" do
  describe "#add_intersection" do
    let(:maze) { GriddedMaze.new }
    before do
      maze.add_node [0,0]
    end

    it "does nothing when adding an empty intersection" do
      maze.add_intersection([0,0],[])
      expect(maze.connections[[0,0]]).to eq []
    end

    it "adds a connection when given some exits" do
      maze.add_intersection([0,0],[[1,0],[0,1]])
      expect(maze.connections[[0,0]].sort).to eq [[1,0],[0,1]].sort

      maze.add_intersection([1,0],[[1,0],[0,1]])
      expect(maze.connections[[1,0]].sort).to eq [[0,0],[2,0],[1,1]].sort

      expect(maze.nodes.sort).to eq [[0,0],[1,0],[0,1],[1,1],[2,0]].sort
    end

    it "adds multiple nodes when given a long vector" do
      maze.add_intersection([0,0],[[2,0]])
      expect(maze.nodes.sort).to eq [[0,0],[1,0],[2,0]].sort
    end

    it "does not duplicate nodes" do
      maze.add_intersection([0,0],[[0,2]])
      maze.add_intersection([0,0],[[0,1]])
      expect(maze.nodes.sort).to eq [[0,0],[0,1],[0,2]].sort
    end

    it "freezes nodes" do
      node = [0,1]
      maze.add_node node
      expect { node << 3 }.to raise_exception
    end
  end
end
