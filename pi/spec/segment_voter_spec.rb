require 'spec_helper'
require 'segment_voter'

describe SegmentVoter do
  it "returns nil for unknown semgents" do
    expect(subject.connected?(Point(0,0),Point(0,1))).to eq nil
  end

  it "returns true after a yes vote" do
    subject.vote_connected(Point(0,0),Point(0,1))
    expect(subject.connected?(Point(0,0),Point(0,1))).to eq true
  end

  it "returns false after a no vote" do
    subject.vote_not_connected(Point(0,0),Point(0,1))
    expect(subject.connected?(Point(0,0),Point(0,1))).to eq false
  end

  it "keeps separate tallies" do
    subject.vote_not_connected(Point(0,0),Point(0,1))
    subject.vote_connected(Point(0,0),Point(1,0))
    expect(subject.connected?(Point(0,0),Point(0,1))).to eq false
    expect(subject.connected?(Point(0,0),Point(1,0))).to eq true
  end

  it "returns nil after 1 yes, 1 no votes" do
    subject.vote_connected(Point(0,0),Point(0,1))
    subject.vote_not_connected(Point(0,0),Point(0,1))
    expect(subject.connected?(Point(0,0),Point(0,1))).to eq nil
  end

  it "works in either order" do
    subject.vote_connected(Point(0,0),Point(0,1))
    subject.vote_not_connected(Point(0,1),Point(0,0))
    expect(subject.connected?(Point(0,0),Point(0,1))).to eq nil
  end
end
