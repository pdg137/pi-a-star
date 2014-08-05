require 'spec_helper'

describe Point do
  let(:p1) { Point.new([1,0]) }
  let(:p2) { Point.new([1,0]) }

  it "behaves well in a hash" do
    h = Hash.new
    h[p1] = 0
    expect(h[p1]).to eq 0
  end

  it "behaves well in a set" do
    s = Set.new
    s << p1
    s << p2
    expect(s.to_a.length).to eq 1
  end

  describe "#+" do
    specify do
      expect(Point(1,0)+Vector(0,1)).to eq Point(1,1)
    end
  end
end
