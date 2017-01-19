#!/usr/bin/env python
#

import sys
import time
import sqlite3
import numpy as np
import matplotlib.pyplot as plt

def read(db_name, con, cur):

    con_r = None
    con_r = sqlite3.connect(db_name)

    if con_r is None:
        raise IOError('Unable to open sqlite database')

    cur_r = con_r.cursor()
    cur_r.execute("SELECT * FROM samples")

    rows = cur_r.fetchall()

    for row in rows:
        sql_insert = "INSERT INTO samples VALUES(NULL,{})".format(','.join(['?']*(len(row)-1)))

        cur.execute(sql_insert, row[1:])

    con.commit()

th_sensors = [
    ('sht31_0', None),
    ('sht31_1', None),
    ('sht31_2', None),
    ('sht31_5', None),
    ('sht31_6', None),
    ('sht31_7', None),
]

# Create new sqlite db for every run (maybe later do day-by-day?)
db_name = time.strftime('th_log_%Y-%m-%d_%H-%M-%S.db', time.localtime())

con = None
con = sqlite3.connect(db_name)

if con is None:
    raise IOError('Unable to open sqlite database')

names = ['timestamp']
for name,sensor in th_sensors:
    names.append('{}_humidity'.format(name))
    names.append('{}_temperature'.format(name))

cur = con.cursor()
cur.execute("CREATE TABLE IF NOT EXISTS samples(id INTEGER PRIMARY KEY,{} INTEGER)".format(' INTEGER, '.join(names)))


read(sys.argv[1], con, cur)
read(sys.argv[2], con, cur)

