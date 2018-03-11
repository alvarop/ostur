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
from flask import Flask, request, session, g, redirect, url_for, abort, \
     render_template, flash


app = Flask(__name__)  # create the application instance :)
app.config.from_object(__name__)  # load config from this file , flaskr.py

# Load default config and override config from an environment variable
app.config.update(dict(
    DATABASE=os.path.join(app.root_path,
                          '/home/alvaro/code/ostur/sw/2.0/brie5.sqlite'),
))

app.config.from_envvar('OSTUR_SETTINGS', silent=True)

data_columns = ['timestamp', 'device_id', 'sample_index', 'sensor_ch',
                'temperature', 'humidity', 'fridge_state', 'flags']


def connect_db():
    """Connects to the specific database."""
    rv = sqlite3.connect(app.config['DATABASE'])
    rv.row_factory = sqlite3.Row
    return rv


def get_db():
    """Opens a new database connection if there is none yet for the
    current application context.
    """
    if not hasattr(g, 'sqlite_db'):
        g.sqlite_db = connect_db()
    return g.sqlite_db


@app.teardown_appcontext
def close_db(error):
    """Closes the database again at the end of the request."""
    if hasattr(g, 'sqlite_db'):
        g.sqlite_db.close()


@app.route('/')
def summary():
    db = get_db()
    cur = db.execute('select * from samples order by timestamp DESC limit 10')
    rows = cur.fetchall()
    samples = []
    channels = {}
    fridge_state = False
    for row in rows:
        sample = {}

        if row['sensor_ch'] in channels:
            continue
        channels[row['sensor_ch']] = {}

        if row['fridge_state'] == 1:
            fridge_state = True

        for key in data_columns:
            val = row[key]
            if key == 'timestamp':
                val = datetime.datetime.fromtimestamp(
                    val).strftime("%Y-%m-%d %H:%M:%S")
            elif key == 'temperature' or key == 'humidity':
                val = float(val) / 100

            sample[key] = val
        samples.append(sample)

    samples = sorted(samples, key=lambda k: k['sensor_ch'])

    return render_template('status.html',
                           samples=samples,
                           fridge_state=fridge_state)


@app.route('/all')
def show_all():
    db = get_db()
    cur = db.execute('select * from samples')
    rows = cur.fetchall()
    samples = []
    for row in rows:
        sample = {}
        for key in data_columns:
            val = row[key]
            if key == 'timestamp':
                val = datetime.datetime.fromtimestamp(
                    val).strftime("%Y-%m-%d %H:%M:%S")
            elif key == 'temperature' or key == 'humidity':
                val = float(val) / 100

            sample[key] = val
        samples.append(sample)

    return render_template('show_samples.html', samples=samples)
