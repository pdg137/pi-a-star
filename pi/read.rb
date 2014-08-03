require_relative 'a_star'

a_star = AStar.new

while true
  sensors = [0,0,0,0,0]
  print "#{distance} #{errors1} #{errors2} #{buttons} #{sensors} #{pos} #{left} #{straight} #{right}     \r"
  sleep(0.1)
end
