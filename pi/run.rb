require_relative 'a_star'
require_relative 'maze_solver'

a_star = AStar.new
MazeSolver.new(a_star).run
