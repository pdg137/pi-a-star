#!/usr/bin/python

from a_star import AStar
import time

a_star = AStar()

delay = 0.3

a_star.leds(0,0,0)
time.sleep(delay)
a_star.leds(1,0,0)
time.sleep(delay)
a_star.leds(1,1,0)
time.sleep(delay)
a_star.leds(0,1,1)
time.sleep(delay)
a_star.leds(0,0,1)
time.sleep(delay)
a_star.leds(0,0,0)
time.sleep(delay)
a_star.leds(0,0,1)
time.sleep(delay)
a_star.leds(0,1,1)
time.sleep(delay)
a_star.leds(1,1,0)
time.sleep(delay)
a_star.leds(1,0,0)
time.sleep(delay)
a_star.leds(0,0,0)
