#!/usr/bin/env python
#

import sys
import time
import sqlite3


# Create new sqlite db for every run (maybe later do day-by-day?)
db_name = sys.argv[1]

con = None
con = sqlite3.connect(db_name)

if con is None:
    raise IOError('Unable to open sqlite database')

cur = con.cursor()

cur.execute("SELECT * FROM samples")

rows = cur.fetchall()

(index, timestamp, h1, t1, t2, t3) = zip(*rows)

# Convert timestamp to hours since first sample
timestamp = [str((x - timestamp[0])/60.0/60.0) for x in timestamp]

# Convert back to units (from units * 10 stored in db)
h1 = [str(x / 10.0) for x in h1]
t1 = [str(x / 10.0) for x in t1]

js_str = 'var trace1 = {\n'
js_str += 'x: [' + ','.join(timestamp) + '],\n'
js_str += 'y: [' + ','.join(h1) + '],\n'
js_str += 'type: \'scatter\'\n'
js_str += '};'

print js_str