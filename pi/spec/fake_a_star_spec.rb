require 'spec_helper'

describe FakeAStar do
  subject do
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

  before do
    subject.goto(Point(0,3),Vector(0,-1))
  end

  it "starts with the given point and direction" do
    expect(subject.pos).to eq Point(0,3)
    expect(subject.vec).to eq Vector(0,-1)
  end

  describe "#turn" do
    specify do
      subject.turn(:left) do |result|
        expect(result).to be_a_kind_of ResponseState::Response
        result.done { }
      end

      expect(subject.vec).to eq Vector(1,0)
    end
  end

  describe "#follow" do
    specify do
      context = nil

      subject.follow do |result|
        expect(result).to be_a_kind_of ResponseState::Response
        result.end { raise }
        result.intersection { context = result.context }
      end

      expect(subject.pos).to eq Point(0,2)
      expect(context[:exits]).to eq [:left,:back]

      subject.turn(:left) do |result|
        result.done { }
      end

      subject.follow do |result|
        result.end { raise }
        result.intersection { context = result.context }
      end

      expect(context[:exits]).to eq [:right,:back]

      subject.turn(:right) do |result|
        result.done { }
      end

      subject.follow do |result|
        result.end { raise }
        result.intersection { context = result.context }
      end

      expect(context[:exits]).to eq [:left,:straight,:back]
    end
  end
end

