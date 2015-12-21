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
    return json.dumps(buttons)

if __name__ == "__main__":
    app.run(host = "0.0.0.0")
