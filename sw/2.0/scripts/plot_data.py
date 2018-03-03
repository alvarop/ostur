#!/usr/bin/env python
"""
Plot csv captured from simple_scan.py
"""

import argparse
import csv
import struct
import time
from numpy import arange
import datetime
import matplotlib.pyplot as plt
from matplotlib.dates import DayLocator, HourLocator, DateFormatter, drange


data_columns = ['timestamp', 'device_id', 'sample_index', 'sensor_ch',
                'temperature', 'humidity', 'fridge_state', 'flags']


parser = argparse.ArgumentParser()

parser.add_argument('--filename',
                    required=True,
                    help='Input filename (csv format)')

args = parser.parse_args()

devices = {}

# Read in CSV data into dictionary
# Organized by device->channel
with open(args.filename, mode='r') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        if row['device_id'] not in devices:
            devices[row['device_id']] = {
            }

        if row['sensor_ch'] not in devices[row['device_id']]:
            devices[row['device_id']][row['sensor_ch']] = {
                'timestamp': [],
                'humidity': [],
                'temperature': [],
            }
        channel = devices[row['device_id']][row['sensor_ch']]

        channel['timestamp'].append(
            datetime.datetime.strptime(row['timestamp'], '%Y-%m-%d %H:%M:%S'))
        channel['temperature'].append(float(row['temperature']))
        channel['humidity'].append(float(row['humidity']))

for device_id, device in devices.items():
    # Two subplots, the axes array is 1-d
    fig, axarr = plt.subplots(2, sharex=True)
    axarr[0].set_title('Ostur Plot (Device {:08X})'.format(int(device_id)))

    for index, channel in device.items():
        axarr[0].plot(
            channel['timestamp'], channel['humidity'], label=str(index))
        axarr[1].plot(
            channel['timestamp'], channel['temperature'], label=str(index))

    axarr[1].set_ylabel('Temperature (C)')
    axarr[0].set_ylabel('% Humidity')

    axarr[0].xaxis.set_major_locator(DayLocator())
    axarr[0].xaxis.set_minor_locator(HourLocator(arange(0, 25, 6)))
    axarr[0].xaxis.set_major_formatter(DateFormatter('%Y-%m-%d %H:%M:%S'))

    legend = axarr[0].legend(loc='best', shadow=True)

    # Make the dates at the bottom pretty
    fig.autofmt_xdate()

    plt.show()
