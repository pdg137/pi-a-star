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

  it "raises an exception if we are off the line" do
    subject.goto(Point(0,2),Vector(0,-1))
    expect {subject.follow {}}.to raise_error FakeAStar::OffLineException
  end

  it "returns approximately 280 ticks per inch" do
    distance = nil

    subject.follow do |result|
      result.end { raise }
      result.intersection { distance = result.context[:distance] }
    end

    expect(distance).to be_within(50).of(280*6)
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
    it "finds exits after a straightaway" do
      context = nil
      subject.goto Point(3,0), Vector(1,0)
      subject.follow do |result|
        result.end { raise }
        result.intersection { context = result.context }
      end
      expect(context[:exits]).to eq [:left, :back]
      expect(context[:distance]).to be_within(50).of(280*6*3)
    end

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

    it "can follow to the end" do
      %i(left right left right left left right straight).each do |turn|
        subject.follow do |result|
          result.end { raise }
          result.intersection { }
        end

        subject.turn(turn) do |result|
          result.done { }
        end
      end

      found_end = nil
      subject.follow do |result|
        result.end { found_end = true }
        result.intersection { raise }
      end

      expect(found_end).to eq true
    end

    it "can follow to the end via a path with straightaways" do
      %i(left right left right left straight left left straight left left).each do |turn|
        subject.follow do |result|
          result.end { raise }
          result.intersection { }
        end

        subject.turn(turn) do |result|
          result.done { }
        end
      end

      found_end = nil
      subject.follow do |result|
        result.end { found_end = true }
        result.intersection { raise }
      end

      expect(found_end).to eq true
    end
  end
end

