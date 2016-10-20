#!/usr/bin/env python
#

import sys
import time
import sqlite3
import numpy as np
import matplotlib.pyplot as plt


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
timestamp = [(x - timestamp[0])/60.0/60.0 for x in timestamp]

# Convert back to units (from units * 10 stored in db)
h1 = [x / 10.0 for x in h1]
t1 = [x / 10.0 for x in t1]


# Show temperature and humidity on same plot with different axes
fig, ax1 = plt.subplots()
ax1.plot(timestamp, h1, 'b-')
ax1.set_xlabel('Time (Hours)')
ax1.set_ylim([0,100])

ax1.set_ylabel('% Humidity', color='b')
for tl in ax1.get_yticklabels():
    tl.set_color('b')

ax2 = ax1.twinx()
ax2.plot(timestamp, t1, 'r-')
# ax2.set_xlabel('time (s)')
ax2.set_ylim([5,25])

ax2.set_ylabel('Temperature (C)', color='r')
for tl in ax2.get_yticklabels():
    tl.set_color('r')

plt.show()