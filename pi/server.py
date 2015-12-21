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

if __name__ == "__main__":
    app.run(host = "0.0.0.0")
