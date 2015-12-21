#!/usr/bin/python

from a_star import AStar
import time
import curses
import curses.wrapper

a_star = AStar()

def run(screen):
  while True:
    c = screen.getch()
    if c == ord('w'):
      a_star.motors(400,400)
    elif c == ord('a'):
      a_star.motors(-400,400)
    elif c == ord('d'):
      a_star.motors(400,-400)
    elif c == ord('s'):
      a_star.motors(-400,-400)
    elif c == ord(' '):
      a_star.motors(0,0)

curses.wrapper(run)
a_star.motors(0,0)
