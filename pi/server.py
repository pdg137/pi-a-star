from flask import Flask
from flask import render_template
app = Flask(__name__)

from a_star import AStar
a_star = AStar()

import json

@app.route("/")
def hello():
    return render_template("index.html")

@app.route("/status.json")
def status():
    buttons = a_star.read_buttons()
    analog = a_star.read_analog()
    battery_millivolts = a_star.read_battery_millivolts()
    data = {
        "buttons": buttons,
        "battery_millivolts": battery_millivolts,
        "analog": analog
    }
    return json.dumps(data)

@app.route("/motors/<left>,<right>")
def motors(left, right):
    a_star.motors(int(left), int(right))
    return ""

@app.route("/leds/<int:led0>,<int:led1>,<int:led2>")
def leds(led0, led1, led2):
    a_star.leds(led0, led1, led2)
    return ""

@app.route("/play_notes/<notes>")
def play_notes(notes):
    a_star.play_notes(notes)
    return ""

if __name__ == "__main__":
    app.run(host = "0.0.0.0")