#!/usr/bin/python

from a_star import AStar

a_star = AStar()

while True:
  buttons = a_star.read_buttons()
  a_star.leds(buttons[2],buttons[0],buttons[1])
