"""
Simple flask app to view latest Ostur data  (read from sqlite db)
To run(debug) using pipenv, run the following:
pipenv shell FLASK_APP=ostur_viewer FLASK_DEBUG=true flask run
NOTE: When you exit, you will still be in a pipenv shell
"""

import os
import sqlite3
import time
import datetime
import statistics
from flask import (
    Flask,
    request,
    session,
    g,
    redirect,
    url_for,
    abort,
    render_template,
    flash,
)


app = Flask(__name__)  # create the application instance :)
app.config.from_object(__name__)  # load config from this file , flaskr.py

# Load default config and override config from an environment variable
app.config.update(
    dict(DATABASE=os.path.join(app.root_path, "/home/alvaro/code/ostur/brie7.db"))
)

app.config.from_envvar("OSTUR_SETTINGS", silent=True)

data_columns = [
    "timestamp",
    "device_id",
    "sample_index",
    "sensor_ch",
    "temperature",
    "humidity",
    "fridge_state",
    "flags",
]


def connect_db():
    """Connects to the specific database."""
    rv = sqlite3.connect(app.config["DATABASE"])
    rv.row_factory = sqlite3.Row
    return rv


def get_db():
    """Opens a new database connection if there is none yet for the
    current application context.
    """
    if not hasattr(g, "sqlite_db"):
        g.sqlite_db = connect_db()
    return g.sqlite_db


@app.teardown_appcontext
def close_db(error):
    """Closes the database again at the end of the request."""
    if hasattr(g, "sqlite_db"):
        g.sqlite_db.close()


def get_last_sample():
    db = get_db()
    cur = db.execute("select * from samples order by timestamp DESC limit 1")
    rows = cur.fetchall()
    return rows[0]


def get_last_door_open():
    last_door_open_str = None
    db = get_db()

    # Experimental door-open detector
    # if the main sensor's humidity goes about 80%, the door has likely opened
    # This only works with low ambient humidity
    # There are better ways to do it with temperature/humidity deltas,
    # but this works for me now :P
    cur = db.execute(
        """select * from samples
        where samples.sensor_ch == 0 and samples.humidity > 8000
        order by timestamp DESC limit 1"""
    )
    rows = cur.fetchall()
    if len(rows) > 0:
        last_door_open_str = datetime.datetime.fromtimestamp(
            rows[0]["timestamp"]
        ).strftime("%Y-%m-%d %H:%M")

    return last_door_open_str


@app.route("/")
def summary():

    # Get latest timestamp to figure out when "one hour ago" was
    latest_timestamp = get_last_sample()["timestamp"]
    last_hour = latest_timestamp - 60 * 60

    # Get last hour of samples
    db = get_db()
    query = "select * from samples where samples.timestamp > {} order by timestamp desc".format(
        last_hour
    )
    cur = db.execute(query)
    rows = cur.fetchall()
    channels = {}
    fridge_state = None
    samples = []
    for row in rows:

        ch = row["sensor_ch"]

        # Only use latest sample for fridge state
        if fridge_state is None:
            if row["fridge_state"] == 1:
                fridge_state = True
            else:
                fridge_state = False

        if ch not in channels:
            channels[ch] = {
                "latest_timestamp": None,
                "temperature": [],
                "humidity": [],
                "latest_temperature": None,
                "latest_humidity": None,
            }

            # Use first sample per channel to figure out latest value
            for key in data_columns:
                if key == "timestamp":
                    channels[ch]["latest_timestamp"] = datetime.datetime.fromtimestamp(
                        row[key]
                    ).strftime("%Y-%m-%d %H:%M:%S")
                elif key == "temperature":
                    channels[ch]["latest_temperature"] = float(row[key]) / 100
                elif key == "humidity":
                    channels[ch]["latest_humidity"] = float(row[key]) / 100

        # Add all the values to lists so we can average them later
        for key in data_columns:
            if key == "temperature":
                channels[ch]["temperature"].append(float(row[key]) / 100)
            elif key == "humidity":
                channels[ch]["humidity"].append(float(row[key]) / 100)

    for ch in channels:
        samples.append(
            {
                "timestamp": channels[ch]["latest_timestamp"],
                "sensor_ch": ch,
                "temperature": channels[ch]["latest_temperature"],
                "humidity": channels[ch]["latest_humidity"],
                "humidity_avg": round(statistics.mean(channels[ch]["humidity"]), 2),
                "temperature_avg": round(
                    statistics.mean(channels[ch]["temperature"]), 2
                ),
            }
        )

    samples = sorted(samples, key=lambda k: k["sensor_ch"])

    last_door_open = get_last_door_open()

    return render_template(
        "status.html",
        samples=samples,
        fridge_state=fridge_state,
        last_door_open=last_door_open,
    )


@app.route("/all")
def show_all():
    db = get_db()
    cur = db.execute("select * from samples")
    rows = cur.fetchall()
    samples = []
    for row in rows:
        sample = {}
        for key in data_columns:
            val = row[key]
            if key == "timestamp":
                val = datetime.datetime.fromtimestamp(val).strftime("%Y-%m-%d %H:%M:%S")
            elif key == "temperature" or key == "humidity":
                val = float(val) / 100

            sample[key] = val
        samples.append(sample)

    return render_template("show_samples.html", samples=samples)
