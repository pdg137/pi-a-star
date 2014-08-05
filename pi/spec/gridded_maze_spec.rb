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

  describe "vectors_to_turn" do
    north = [0,1]
    south = [0,-1]
    east = [1,0]
    west = [-1,0]

    it "identifies a turn from north to west as left" do
      expect(GriddedMaze.vectors_to_turn(north,west)).to eq :left
    end

    it "identifies a turn from west to north as right" do
      expect(GriddedMaze.vectors_to_turn(west,north)).to eq :right
    end

    it "identifies a turn from west to east as back" do
      expect(GriddedMaze.vectors_to_turn(west,east)).to eq :back
    end

    it "identifies a turn from south to south as straight" do
      expect(GriddedMaze.vectors_to_turn(south,south)).to eq :straight
    end
  end

  context "simple gridded maze" do
    # #-#-#
    # |   |
    # #-#-#
    # |
    # #-#

    let(:maze) { GriddedMaze.new }

    before do
      [[0,2], [1,2], [2,2],
       [0,1], [1,1], [2,1],
       [0,0], [1,0]].each do |node|
        maze.add_node node
      end

      maze.connect [0,0], [1,0]

      maze.connect [0,0], [0,1]
      maze.connect [0,1], [1,1]
      maze.connect [1,1], [2,1]

      maze.connect [0,1], [0,2]
      maze.connect [2,1], [2,2]
      maze.connect [0,2], [1,2]
      maze.connect [1,2], [2,2]
    end

    specify do
      expect(maze.get_path [0,0], [1,2]).to eq [[0,0],[0,1],[0,2],[1,2]]
    end

    specify do
      turning_path = maze.get_turning_path [-1,0], [1,0], [2,1]
      expect(turning_path[:turns]).to eq [:right, :right]
      expect(turning_path[:initial_turn]).to eq :straight
    end

    specify do
      turning_path = maze.get_turning_path [0,1], [0,0], [1,2]
      expect(turning_path[:turns]).to eq [:straight, :right]
      expect(turning_path[:initial_turn]).to eq :straight
    end

    specify do
      turning_path = maze.get_turning_path [0,-1], [0,0], [1,2]
      expect(turning_path[:turns]).to eq [:straight, :right]
      expect(turning_path[:initial_turn]).to eq :back
    end
  end
end
