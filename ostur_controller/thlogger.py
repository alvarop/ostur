#!/usr/bin/env python
#

import sys
import time
import sqlite3
from Ostur import Ostur

def log(cur, con, controller):
    while True:
        try:
            line = []
            sensors = controller.read_sensors()

            line.append(int(sensors[0]))
            for index in range(1, len(sensors)):
                value = float(sensors[index])

                # Saving integer data to reduce SQLite db size
                # Storing units * 100 to keep 2 significant figures
                line.append(int(value * 100))

            sql_insert = "INSERT INTO samples VALUES(NULL,{})".format(','.join(['?']*len(line)))

            cur.execute(sql_insert, line)
            con.commit()
            print line
        except IndexError as e:
            print "Uh oh, index error!"
            print e
        except IOError as e:
            print "Uh oh, IO error!"
            print e

controller = Ostur('/dev/ttyUSB0', 115200)
controller.stop_sampling()
controller.close()

controller = Ostur('/dev/ttyUSB0', 115200)

# Create new sqlite db for every run (maybe later do day-by-day?)
db_name = time.strftime('th_log_%Y-%m-%d_%H-%M-%S.db', time.localtime())

con = None
con = sqlite3.connect(db_name)

if con is None:
    raise IOError('Unable to open sqlite database')

print('Saving to %s', db_name)

controller.set_localtime()

names = controller.start_sampling()

cur = con.cursor()
cur.execute("CREATE TABLE IF NOT EXISTS samples(id INTEGER PRIMARY KEY,{} INTEGER)".format(' INTEGER, '.join(names)))

print(names)
log(cur, con, controller)