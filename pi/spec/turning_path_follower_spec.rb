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
end
