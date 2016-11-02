#!/usr/bin/env python
#

import urllib, json
import sys
import time
import numpy as np
import sqlite3

NUM_SAMPLES = 10

temps = [0] * NUM_SAMPLES

TEMP_SETTING = 15

def fridge_control(control_server, enable=False):
    control_url = control_server + '/relay/fridge/'
    if enable is True:
        control_url += 'enable'
    else:
        control_url += 'disable'

    response = urllib.urlopen(control_url)
    success = json.loads(response.read())
    print control_url
    print success

def control(control_server, current_temp):
    # Remove oldest sample
    temps.pop(0)
    temps.append(current_temp)
    temp = np.mean(temps)

    print temps, temp

    if temp > TEMP_SETTING:
        fridge_control(control_server, True)
    else:
        fridge_control(control_server, False)


def log(control_server, cur, con):
    while True:
        try:
            response = urllib.urlopen(control_server + '/sensors')
            sensors = json.loads(response.read())

            control(control_server, sensors['temp0'])

            line = []

            # Saving integer data to reduce SQLite db size
            line.append(str(int(sensors['time'])))

            # Storing units * 10 to keep 1 significant figure
            line.append(str(int(sensors['humidity'] * 10)))
            line.append(str(int(sensors['temp0'] * 10)))
            line.append(str(int(sensors['temp1'] * 10)))
            line.append(str(int(sensors['temp2'] * 10)))

            line_str = ','.join(line)

            cur.execute("INSERT INTO samples VALUES(NULL, " + line_str + " )")
            con.commit()
            print line_str

        except ValueError:
            print('# Error decoding JSON')

        time.sleep(1)


# Create new sqlite db for every run (maybe later do day-by-day?)
db_name = time.strftime('ostur_control_log_%Y-%m-%d_%H-%M-%S.db', time.localtime())

con = None
con = sqlite3.connect(db_name)

if con is None:
    raise IOError('Unable to open sqlite database')

cur = con.cursor()
cur.execute("CREATE TABLE IF NOT EXISTS samples(id INTEGER PRIMARY KEY, timestamp INTEGER, t1 INTEGER, h1 INTEGER, t2 INTEGER, t3 INTEGER)")

control_server = 'http://' + sys.argv[1]

try:
    response = urllib.urlopen(control_server + '/info')
    info = json.loads(response.read())
    print info
    log(control_server, cur, con)

except IOError:
    print('Unable to connect to control server')

