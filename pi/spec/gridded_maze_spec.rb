require 'spec_helper'

describe "GriddedMaze" do
  describe "#add_intersection" do
    let(:maze) { GriddedMaze.new }
    before do
      maze.add_node [0,0]
    end

    it "represents nodes as Point objects" do
      maze.nodes.to_a[0].is_a? Point
    end

    it "does nothing when adding an empty intersection" do
      maze.add_intersection(Point(0,0),[])
      expect(maze.connections[Point(0,0)]).to eq [].to_set
    end

    it "adds a connection when given some exits" do
      maze.add_intersection(Point(0,0),[Vector(1,0),Vector(0,1)])
      expect(maze.connections[Point(0,0)]).to eq [Point(1,0),Point(0,1)].to_set

      maze.add_intersection(Point(1,0),[Vector(1,0),Vector(0,1)])
      expect(maze.connections[Point(1,0)]).to eq [Point(0,0),Point(2,0),Point(1,1)].to_set

      expect(maze.nodes).to eq [[0,0],[1,0],[0,1],[1,1],[2,0]].map { |x| Point.new x }.to_set
    end

    it "adds multiple nodes when given a long vector" do
      maze.add_intersection(Point(0,0),[Vector(2,0)])
      expect(maze.nodes).to eq [[0,0],[1,0],[2,0]].map { |x| Point.new x }.to_set
    end

    it "does not duplicate nodes" do
      maze.add_intersection(Point(0,0),[Vector(0,2)])
      maze.add_intersection(Point(0,0),[Vector(0,1)])
      expect(maze.nodes).to eq [[0,0],[0,1],[0,2]].map { |x| Point.new x }.to_set
    end
  end

  describe "vectors_to_turn" do
    north = Vector 0,1
    south = Vector 0,-1
    east = Vector 1,0
    west = Vector -1,0

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

      maze.connect Point(0,0), Point(1,0)

      maze.connect Point(0,0), Point(0,1)
      maze.connect Point(0,1), Point(1,1)
      maze.connect Point(1,1), Point(2,1)

      maze.connect Point(0,1), Point(0,2)
      maze.connect Point(2,1), Point(2,2)
      maze.connect Point(0,2), Point(1,2)
      maze.connect Point(1,2), Point(2,2)
    end

    specify do
      expect(maze.get_path Point(0,0), Point(1,2)).to eq [[0,0],[0,1],[0,2],[1,2]].map { |x| Point.new x }
    end

    it do
      turning_path = maze.get_turning_path Vector(-1,0), Point(1,0), Point(2,1)
      expect(turning_path[:turns]).to eq [:right, :right]
      expect(turning_path[:initial_turn]).to eq :straight
    end

    it do
      turning_path = maze.get_turning_path Vector(0,1), Point(0,0), Point(1,2)
      expect(turning_path[:turns]).to eq [:straight, :right]
      expect(turning_path[:initial_turn]).to eq :straight
    end

    it do
      turning_path = maze.get_turning_path Vector(0,-1), Point(0,0), Point(1,2)
      expect(turning_path[:turns]).to eq [:straight, :right]
      expect(turning_path[:initial_turn]).to eq :back
    end
  end
end
