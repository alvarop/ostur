import os
import time
import socket
from datetime import datetime, timedelta
from flask import Flask, request, g, render_template, jsonify
from ostur.osturdb import OsturDB

app = Flask(__name__)

app.config.from_object(__name__)  # load config from this file , flaskr.py

# Load default config and override config from an environment variable
app.config.update(dict(DATABASE=os.getenv("DATABASE")))

hostname = socket.gethostname()


def get_db():
    """Opens a new database connection if there is none yet for the
    current application context.
    """
    if not hasattr(g, "sqlite_db"):
        g.sqlite_db = OsturDB(app.config["DATABASE"])
    return g.sqlite_db


@app.teardown_appcontext
def close_db(error):
    """Closes the database again at the end of the request."""
    if hasattr(g, "sqlite_db"):
        g.sqlite_db.close()


def get_latest_sample():
    """ Get latest data """

    # Get last sample
    db = get_db()

    sample = {"hotsname": hostname, "devices": {}}

    for device in db.devices:
        # Convert the units
        sample["devices"][device] = {}
        rows = db.get_records("day", order="desc", limit=1, uid=device)
        data = {}
        for key, val in rows[0]._asdict().items():
            if key == "timestamp":
                data[key] = datetime.fromtimestamp(val).strftime("%Y-%m-%d %H:%M:%S")
            elif val == None:
                data[key] = 0
            else:
                data[key] = round(float(val), 2)

        sample["devices"][device] = data

    # Past day
    now = datetime.fromtimestamp(int(time.time()))
    end_time = start_time = time.mktime(now.timetuple())

    return sample


@app.route("/latest")
def latest_json():
    return jsonify(get_latest_sample())


@app.route("/")
def summary():
    sample = get_latest_sample()

    return render_template("status.html", hostname=hostname)


days = ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]


def get_json_str(start_date, end_date, table="day"):
    """ Get data for the specified period """

    db = get_db()

    plot = {"hotsname": hostname, "devices": {}}

    for device in db.devices:
        rows = db.get_records(
            table, start_date=start_date, end_date=end_date, uid=device
        )
        data = {}

        # Ignore devices with no samples in the current range
        if len(rows) == 0:
            continue

        col_names = rows[0]._asdict().keys()

        for name in col_names:
            data[name] = []

        # Create lists with each data type and make timestamp pretty
        for row in rows:
            for name in col_names:
                if name == "timestamp":
                    data[name].append(
                        datetime.fromtimestamp(getattr(row, name)).strftime(
                            "%Y-%m-%d %H:%M:%S"
                        )
                    )
                elif name == "uid" or name == "id":
                    continue
                else:
                    if getattr(row, name) is None:
                        data[name].append(0)
                    else:
                        data[name].append(round(getattr(row, name), 3))

        plot["devices"][device] = data

        plot["start_date"] = datetime.fromtimestamp(start_date).strftime(
            "%Y-%m-%d %H:%M:%S"
        )
        plot["end_date"] = datetime.fromtimestamp(end_date).strftime(
            "%Y-%m-%d %H:%M:%S"
        )

    return jsonify(plot)

@app.route("/rename_device")
def rename_device():
    db = get_db()

    uid = int(request.args.get('uid'))
    new_name = request.args.get('name')

    if uid is None or new_name is None:
        return "ERR"
    elif uid not in db.devices:
        return "ERR 2"
    else:
        print("renaming {} to {}".format(uid, new_name))
        db.rename_device(uid, new_name)
        return "OK"

@app.route("/json/devices")
def get_devices():
    db = get_db()

    return jsonify(db.devices)


@app.route("/json/day")
def json_day_str():
    # time.time() is utc time, but now is a "naive"
    # datetime object in current timezone
    now = datetime.fromtimestamp(int(time.time()))

    # Start 24 hours before the next full hour
    start_time = time.mktime(
        (
            now.replace(minute=0, second=0) + timedelta(hours=1) - timedelta(days=1)
        ).timetuple()
    )

    end_time = time.mktime(now.timetuple())

    return get_json_str(start_time, end_time, "day")


@app.route("/json/week")
def json_week_str():
    # time.time() is utc time, but now is a "naive"
    # datetime object in current timezone
    now = datetime.fromtimestamp(int(time.time()))

    # Round to the full day, start 7 days ago
    start_time = time.mktime(
        (
            now.replace(hour=0, minute=0, second=0)
            + timedelta(days=1)
            - timedelta(weeks=1)
        ).timetuple()
    )

    end_time = time.mktime(now.timetuple())

    return get_json_str(start_time, end_time, "week")


@app.route("/json/month")
def json_month_str():
    # time.time() is utc time, but now is a "naive"
    # datetime object in current timezone
    now = datetime.fromtimestamp(int(time.time()))

    # TODO - round to the month?
    # Round to the full day, start 31 days ago
    start_time = time.mktime(
        (
            now.replace(hour=0, minute=0, second=0)
            + timedelta(days=1)
            - timedelta(days=31)
        ).timetuple()
    )

    end_time = time.mktime(now.timetuple())

    return get_json_str(start_time, end_time, "month")


@app.route("/plots")
def test():
    return render_template("plots.html", hostname=hostname)
