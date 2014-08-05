require 'spec_helper'

INF = Float::INFINITY

describe Maze do
  let(:nodes) { [1,2,3,4] }
  let(:connections) {
    {
      1 => [2],
      2 => [1,3,4],
      3 => [2,4],
      4 => [2,3]
    }
  }
  let(:maze) { Maze.new(nodes,connections) }

  describe "#solve" do
    it "solves to node 4" do
      expect(maze.solve(4)).to eq [2,1,1,0]
    end

    it "solves to node 1" do
      expect(maze.solve(1)).to eq [0,1,2,2]
    end
  end

  describe "#initial_score" do
    it "sets the target node to 0, others to INF" do
      expect(maze.initial_score(4)).to eq [INF,INF,INF,0]
      expect(maze.initial_score(1)).to eq [0,INF,INF,INF]
    end
  end

  describe "#closest" do
    it "returns the only node in a trivial case" do
      expect(maze.closest([1],[INF,INF,INF,INF])).to eq 1
    end

    it "returns the node with lowest score" do
      expect(maze.closest([1,2,3,4],[INF,1,3,2])).to eq 2
    end
  end

  describe "#check" do
    specify "given 1, it sets the score on node 2" do
      score = [0,INF,INF,INF]
      maze.check(score,1)
      expect(score).to eq [0,1,INF,INF]
    end

    specify "it adds 1 to the score" do
      score = [5,INF,INF,INF]
      maze.check(score,1)
      expect(score).to eq [5,6,INF,INF]
    end

    specify "does not overwrite lower scores" do
      score = [5,5,INF,INF]
      maze.check(score,1)
      expect(score).to eq [5,5,INF,INF]
    end

    specify "spreads to all connected nodes" do
      score = [INF,0,INF,INF]
      maze.check(score,2)
      expect(score).to eq [1,0,1,1]
    end
  end
end
