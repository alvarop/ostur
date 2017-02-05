#!/usr/bin/env python
#

import sys
import time
import sqlite3
import numpy as np
import datetime
import matplotlib.pyplot as plt
from matplotlib.dates import DayLocator, HourLocator, DateFormatter, drange
from numpy import arange


# Create new sqlite db for every run (maybe later do day-by-day?)
db_name = sys.argv[1]

con = None
con = sqlite3.connect(db_name)

if con is None:
    raise IOError('Unable to open sqlite database')

cur = con.cursor()
cur.execute("SELECT * FROM samples")

rows = cur.fetchall()

(index, timestamp, t0, h0, t1, h1, t2, h2) = zip(*rows)

# Convert timestamp to datetime
dates = [datetime.datetime.fromtimestamp(x/1000.0) for x in timestamp]

# Convert back to units (from units * 100 stored in db)
h0 = [x / 100.0 for x in h0]
h1 = [x / 100.0 for x in h1]
h2 = [x / 100.0 for x in h2]

t0 = [x / 100.0 for x in t0]
t1 = [x / 100.0 for x in t1]
t2 = [x / 100.0 for x in t2]

# Two subplots, the axes array is 1-d
fig, axarr = plt.subplots(2, sharex=True)
axarr[0].set_title('Cheese cave controller!')

axarr[0].plot(dates, h0, 'b', label='fridge')
axarr[0].plot(dates, h1, 'g', label='outside')
axarr[0].plot(dates, h2, 'r', label='cheese')
axarr[0].set_ylabel('% Humidity')

axarr[1].plot(dates, t0, 'b', label='fridge')
axarr[1].plot(dates, t1, 'g', label='outside')
axarr[1].plot(dates, t2, 'r', label='cheese')
axarr[1].set_ylabel('Temperature (C)')

axarr[0].xaxis.set_major_locator(DayLocator())
axarr[0].xaxis.set_minor_locator(HourLocator(arange(0, 25, 6)))
axarr[0].xaxis.set_major_formatter(DateFormatter('%Y-%m-%d'))

legend = axarr[0].legend(loc='best', shadow=True)

# Make the dates at the bottom pretty
fig.autofmt_xdate()

plt.show()