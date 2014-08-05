require 'complex'

class Vector
  attr_reader :complex

  def initialize(*args)
    args = args[0] if args.length == 1 && args[0].is_a?(Array)
    @complex = Complex *args
  end

  def hash
    complex.hash
  end

  def ==(other)
    complex == other.complex
  end

  def eql?(other)
    complex.eql? other.complex
  end

  def unit
    assert_cardinal
    if x > 0
      Vector(1,0)
    elsif y > 0
      Vector(0,1)
    elsif x < 0
      Vector(-1,0)
    else
      Vector(0,-1)
    end
  end

  def x
    complex.real
  end

  def y
    complex.imag
  end

  def assert_cardinal
    if (x != 0 && y != 0) || (x == 0 && y == 0)
      raise "only works with cardinal directions (complex = #{complex})"
    end
  end

  def length
    assert_cardinal
    if x != 0
      x.abs
    else
      y.abs
    end
  end

  def inspect
    "<#{x},#{y}>"
  end
end

def Vector(*args)
  Vector.new *args
end
