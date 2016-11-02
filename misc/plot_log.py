#!/usr/bin/env python

import sys
import csv
import numpy as np
import matplotlib.pyplot as plt

index = []
h1 = []
t1 = []
t2 = []
t3 = []


with open(sys.argv[1], 'rb') as csvfile:
	reader = csv.reader(csvfile)
	next(csvfile)
	for row in reader:
		index.append(int(row[0])/60.0/60.0)
		h1.append(row[1])
		t1.append(row[2])
		t2.append(row[3])
		t3.append(row[4])


fig, ax1 = plt.subplots()
ax1.plot(index, h1, 'b-')
ax1.set_xlabel('time (H)')
ax1.set_ylim([0,100])


ax1.set_ylabel('Humidity (%)', color='b')
for tl in ax1.get_yticklabels():
    tl.set_color('b')

ax2 = ax1.twinx()
ax2.plot(index, t1, 'r-')
ax2.plot(index, t2, 'r-')
ax2.plot(index, t3, 'r-')
ax2.set_ylim([0,30])
# ax1.set_xlim([0,0.1])

ax2.set_ylabel('Temperature (C)', color='r')
for tl in ax2.get_yticklabels():
    tl.set_color('r')

plt.show()
