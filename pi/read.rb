require_relative 'a_star'

a_star = AStar.new

while true
  (count1, count2, errors1, errors2) = a_star.get_report
  print "#{count1} #{count2} #{errors1} #{errors2}     \r"
  sleep(0.1)
end
