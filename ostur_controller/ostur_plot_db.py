#!/usr/bin/env python
#

import argparse
import operator
import sys
import time
import sqlite3
import numpy as np
import datetime
import matplotlib.pyplot as plt
from matplotlib.dates import DayLocator, HourLocator, DateFormatter, drange
from numpy import arange


parser = argparse.ArgumentParser()
parser.add_argument('--avg_window',
                    default=1,
                    type=int,
                    help='Number of samples to average')
parser.add_argument('--db',
                    required=True,
                    help='Serial/usb device to connect to')
args = parser.parse_args()

con = None
con = sqlite3.connect(args.db)

if con is None:
    raise IOError('Unable to open sqlite database')

cur = con.cursor()
cur.execute("SELECT * FROM samples")

rows = []
subrows = []

# Average avg_window samples before plotting
while 1:
    row = cur.fetchone()
    if row is not None:
        subrows.append(row)

    # Make sure we don't ignore the last samples either
    if len(subrows) == args.avg_window or (row is None and len(subrows) > 0):
        tmp_array = np.array(subrows)
        rows.append(np.around(np.mean(tmp_array, axis=0)).tolist())
        subrows = []

    if row is None:
        break

(index, timestamp, t0, h0, t1, h1, t2, h2, t3, h3) = zip(*rows)

# Convert timestamp to datetime
dates = [datetime.datetime.fromtimestamp(x/1000.0) for x in timestamp]

# Convert back to units (from units * 100 stored in db)
h0 = [x / 100.0 for x in h0]
h1 = [x / 100.0 for x in h1]
h2 = [x / 100.0 for x in h2]
h3 = [x / 100.0 for x in h3]

t0 = [x / 100.0 for x in t0]
t1 = [x / 100.0 for x in t1]
t2 = [x / 100.0 for x in t2]
t3 = [x / 100.0 for x in t3]

# Two subplots, the axes array is 1-d
fig, axarr = plt.subplots(2, sharex=True)
axarr[0].set_title('Cheese cave controller!')

axarr[0].plot(dates, h0, 'b', label='fridge')
axarr[0].plot(dates, h1, 'g', label='outside')
axarr[0].plot(dates, h2, 'r', label='cheese1')
axarr[0].plot(dates, h3, 'y', label='cheese2')
axarr[0].set_ylabel('% Humidity')

axarr[1].plot(dates, t0, 'b', label='fridge')
axarr[1].plot(dates, t1, 'g', label='outside')
axarr[1].plot(dates, t2, 'r', label='cheese1')
axarr[1].plot(dates, t3, 'y', label='cheese2')
axarr[1].set_ylabel('Temperature (C)')

axarr[0].xaxis.set_major_locator(DayLocator())
axarr[0].xaxis.set_minor_locator(HourLocator(arange(0, 25, 6)))
axarr[0].xaxis.set_major_formatter(DateFormatter('%Y-%m-%d %H:%M:%S'))

legend = axarr[0].legend(loc='best', shadow=True)

# Make the dates at the bottom pretty
fig.autofmt_xdate()

plt.show()
