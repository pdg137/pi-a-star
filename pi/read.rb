require_relative 'a_star'

a_star = AStar.new

while true
  sensors = [0,0,0,0,0]
  (count1, count2, errors1, errors2, buttons,
   sensors[0],sensors[1],sensors[2],sensors[3],sensors[4]) = a_star.get_report
  print "#{count1} #{count2} #{errors1} #{errors2} #{buttons} #{sensors}    \r"
  sleep(0.1)
end
