require_relative 'a_star'

a_star = AStar.new

while true
  report = a_star.get_report
  print "#{report.distance} #{report.sensors}     \r"
  sleep(0.1)
end
