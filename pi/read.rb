require_relative 'lib/a_star'

a_star = AStar.new

while true
  report = a_star.get_report
  print "#{report.distance} #{report.sensors} #{[report.button0?, report.button1?, report.button2?]}     \r"
  sleep(0.1)
end
