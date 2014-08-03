require_relative 'a_star'

a_star = AStar.new

while true
  report = a_star.get_report
  print "#{report.distance} #{report.follow_state} #{report.left} #{report.straight} #{report.right}     \r"
  sleep(0.1)
end
