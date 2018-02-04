#!/usr/bin/env python
#

import sys
import time
import sqlite3
import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime

def timestampFromFilename(filename):
    dt = datetime.strptime(filename,'th_log_%Y-%m-%d_%H-%M-%S.db')
    return int(time.mktime(dt.timetuple()))

def read(db_name, con, cur, actual_start_time = 0):

    con_r = sqlite3.connect(db_name)

    if con_r is None:
        raise IOError('Unable to open sqlite database')

    cur_r = con_r.cursor()
    cur_r.execute("SELECT * FROM samples")

    rows = cur_r.fetchall()

    first_timestamp = rows[0][1]

    for row in rows:
        row = list(row)
        row[1] = row[1] - first_timestamp + actual_start_time * 1000
        sql_insert = "INSERT INTO samples VALUES(NULL,{})".format(','.join(['?']*(len(row)-1)))
        cur.execute(sql_insert, row[1:])

    con.commit()

NUM_SENSORS = 3

# Create new sqlite db for every run (maybe later do day-by-day?)
db_name = time.strftime('merged.db', time.localtime())

con = None
con = sqlite3.connect(db_name)

if con is None:
    raise IOError('Unable to open sqlite database')

names = ['timestamp']
for sensor in range(NUM_SENSORS):
    names.append('sensor{}_t'.format(sensor))
    names.append('sensor{}_h'.format(sensor))

print names

cur = con.cursor()
cur.execute("CREATE TABLE IF NOT EXISTS samples(id INTEGER PRIMARY KEY,{} INTEGER)".format(' INTEGER, '.join(names)))

for file in range(1,len(sys.argv)):
    filename = sys.argv[file]
    timestamp = timestampFromFilename(filename)
    print("Reading file %s starting at %d" % (filename,timestamp))
    read(filename, con, cur, timestamp)
