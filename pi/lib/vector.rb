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
    if other.is_a? Vector
      complex == other.complex
    else
      super other
    end
  end

  def eql?(other)
    if other.is_a? Vector
      complex.eql? other.complex
    else
      super other
    end
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

  alias_method :to_s, :inspect

  def turn(dir)
    Vector complex*case dir
                   when :left
                     Complex::I
                   when :right
                     -Complex::I
                   when :straight
                     1
                   when :back
                     -1
                   else
                     raise "invalid dir #{dir.inspect}"
                   end
  end

  def dir_to(finish)
    self.assert_cardinal
    finish.assert_cardinal

    a = [Vector(1,0),Vector(0,1),Vector(-1,0),Vector(0,-1)].index self
    b = [Vector(1,0),Vector(0,1),Vector(-1,0),Vector(0,-1)].index finish

    left_amount = b - a
    left_amount += 4 if left_amount < 0

    [:straight, :left, :back, :right][left_amount]
  end

  def length
    complex.abs
  end
end

def Vector(*args)
  Vector.new *args
end
