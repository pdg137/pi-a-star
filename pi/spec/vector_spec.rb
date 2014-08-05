require 'spec_helper'

describe Vector do
  describe "#turn" do
    specify do
      expect(Vector(3,0).turn(:left)).to eq Vector(0,3)
    end
    specify do
      expect(Vector(-1,0).turn(:right)).to eq Vector(0,1)
    end
    specify do
      expect(Vector(0,2).turn(:straight)).to eq Vector(0,2)
    end
    specify do
      expect(Vector(0,2).turn(:back)).to eq Vector(0,-2)
    end
  end

  describe "#==" do
    specify do
      expect(Vector(0,0)).to_not eq :left
    end
  end


  describe "vectors_to_turn" do
    north = Vector 0,1
    south = Vector 0,-1
    east = Vector 1,0
    west = Vector -1,0

    it "identifies a turn from north to west as left" do
      expect(north.dir_to(west)).to eq :left
    end

    it "identifies a turn from west to north as right" do
      expect(west.dir_to(north)).to eq :right
    end

    it "identifies a turn from west to east as back" do
      expect(west.dir_to(east)).to eq :back
    end

    it "identifies a turn from south to south as straight" do
      expect(south.dir_to(south)).to eq :straight
    end
  end
end
