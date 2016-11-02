#!/usr/bin/env python
#

import sys
import time
import sqlite3
from silta import stm32f407
from am2315 import AM2315
from mcp970x import MCP9701
from si7021 import SI7021
from htu21d import HTU21D
from sht31 import SHT31

def log(cur, con, th_sensors):
    while True:
        try:
            line = []
            
            line.append(int(time.time()))
            for name,sensor in th_sensors:
                h, t = sensor.read()
            
                # Saving integer data to reduce SQLite db size
                # Storing units * 10 to keep 1 significant figure
                line.append(int(h * 10))
                line.append(int(t * 10))

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

        time.sleep(1)

bridge = stm32f407.bridge(sys.argv[1])

th_sensors = [
    ('si7021', SI7021(bridge)),
    ('htu21d', HTU21D(bridge)),
    ('sht31', SHT31(bridge)),
    ('am2315', AM2315(bridge)),
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

print(names)
log(cur, con, th_sensors)