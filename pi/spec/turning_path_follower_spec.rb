require 'spec_helper'
require 'turning_path_follower'

def yield_arrays(follower, path)
  follower.compute(path) do |x,y|
    yield [x,y]
  end
end

describe TurningPathFollower do
  let(:unit) { 1800 }
  let(:limit) { 300 }
  subject { TurningPathFollower.new(unit, limit) }

  specify do
    expect { |b|
      yield_arrays(subject, [:left,:right], &b)
    }.to yield_successive_args( [:left, limit],
                                [:right, limit] )
  end

  it "simplifies straights and nones" do
    expect { |b|
      yield_arrays(subject, [:left,:straight,:none,:right], &b)
    }.to yield_successive_args( [:left, unit*2+limit],
                                [:right, limit] )
  end

  it "does no more than 6 in a group if possible" do
    expect { |b|
      yield_arrays(subject, [:left,:straight,:straight,:straight,:straight,:straight,:straight,:right], &b)
    }.to yield_successive_args( [:left, unit*5+limit],
                                [:straight, limit],
                                [:right, limit])
  end

  it "makes the initial group smaller if necessary to not start with :none" do
    expect { |b|
      yield_arrays(subject, [:left,:straight,:straight,:straight,:none,:none,:none,:right], &b)
    }.to yield_successive_args( [:left, unit*2+limit],
                                [:straight, unit*3+limit],
                                [:right, limit])
  end

  context "path with a 6x straight segment including some :none" do
    let(:path) { [:right, :none, :straight, :none, :straight, :none, :right] }

    it "does not attempt to split up the straightaway" do
      expect { |b|
        yield_arrays(subject, path, &b)
      }.to yield_successive_args( [:right, unit*5+limit],
                                  [:right, limit]
                                  )
    end
  end

  context "path with a 7x straight segment including some :none" do
    let(:path) { [:right, :none, :straight, :none, :straight, :none, :none, :right] }

    it "does not attempt to split up the straightaway" do
      expect { |b|
        yield_arrays(subject, path, &b)
      }.to yield_successive_args( [:right, unit*3+limit],
                                  [:straight, unit*2+limit],
                                  [:right, limit]
                                  )
    end
  end
end
