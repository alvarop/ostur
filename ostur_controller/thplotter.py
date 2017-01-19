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

(index, timestamp, h0, t0, h1, t1, h2, t2, h3, t3, h4, t4, h5, t5) = zip(*rows)

# Convert timestamp to hours since first sample
timestamp = [(x - timestamp[0])/60.0/60.0 for x in timestamp]

# Convert back to units (from units * 10 stored in db)
h0 = [x / 10.0 for x in h0]
h1 = [x / 10.0 for x in h1]
h2 = [x / 10.0 for x in h2]
h3 = [x / 10.0 for x in h3]
h4 = [x / 10.0 for x in h4]
h5 = [x / 10.0 for x in h5]

t0 = [x / 10.0 for x in t0]
t1 = [x / 10.0 for x in t1]
t2 = [x / 10.0 for x in t2]
t3 = [x / 10.0 for x in t3]
t4 = [x / 10.0 for x in t4]
t5 = [x / 10.0 for x in t5]

# Two subplots, the axes array is 1-d
f, axarr = plt.subplots(2, sharex=True)
axarr[0].plot(timestamp, h0)
axarr[0].plot(timestamp, h1)
axarr[0].plot(timestamp, h2)
axarr[0].plot(timestamp, h3)
axarr[0].plot(timestamp, h4)
axarr[0].plot(timestamp, h5)
axarr[0].set_ylim([0,100])
# axarr[0].set_xlim([0,3])
axarr[0].set_title('Sharing X axis')
axarr[1].plot(timestamp, t0)
axarr[1].plot(timestamp, t1)
axarr[1].plot(timestamp, t2)
axarr[1].plot(timestamp, t3)
axarr[1].plot(timestamp, t4)
axarr[1].plot(timestamp, t5)
axarr[1].set_ylim([10,25])

# Show temperature and humidity on same plot with different axes
# fig, ax1 = plt.subplots()
# ax1.plot(timestamp, h1, 'b-')
# ax1.set_xlabel('Time (Hours)')
# ax1.set_ylim([0,100])

# ax1.set_ylabel('% Humidity', color='b')
# for tl in ax1.get_yticklabels():
#     tl.set_color('b')

# ax2 = ax1.twinx()
# ax2.plot(timestamp, t1, 'r-')
# # ax2.set_xlabel('time (s)')
# ax2.set_ylim([5,25])

# ax2.set_ylabel('Temperature (C)', color='r')
# for tl in ax2.get_yticklabels():
#     tl.set_color('r')

plt.show()