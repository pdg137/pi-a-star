require_relative 'a_star'

a_star = AStar.new

while true
  sensors = [0,0,0,0,0]
  (distance, errors1, errors2, buttons,
   sensors[0],sensors[1],sensors[2],sensors[3],sensors[4],
   pos, left, straight, right) = a_star.get_report
  print "#{distance} #{errors1} #{errors2} #{buttons} #{sensors} #{pos} #{left} #{straight} #{right}     \r"
  sleep(0.1)
end
