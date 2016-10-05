#!/usr/bin/env python
#

import urllib, json
import sys
import time
import sqlite3

def log(control_server, cur, con):
    while True:
        try:
            response = urllib.urlopen(control_server + '/sensors')
            sensors = json.loads(response.read())

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
db_name = time.strftime('ostur_log_%Y-%m-%d_%H-%M-%S.db', time.localtime())

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

