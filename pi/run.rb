require_relative 'a_star'

a_star = AStar.new
a_star.set_motors(400,400);

array = (0..400).to_a + (0..400).to_a.reverse + (-400..0).to_a.reverse + (-400..0).to_a

array.each do |speed|
  a_star.set_motors(speed, speed)
end

a_star.set_motors(0,0);

