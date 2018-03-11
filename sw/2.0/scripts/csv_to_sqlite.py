#!/usr/bin/env python
"""
Plot csv captured from simple_scan.py
"""

import argparse
import csv
import struct
import time
import datetime
import sqlite3

data_columns = ['timestamp', 'device_id', 'sample_index', 'sensor_ch',
                'temperature', 'humidity', 'fridge_state', 'flags']


parser = argparse.ArgumentParser()

parser.add_argument('--csvfile',
                    required=True,
                    help='Input filename (csv format)')

parser.add_argument('--db',
                    default=None,
                    help='Sqlite db filename')

args = parser.parse_args()

devices = {}


# Read in CSV data into dictionary
# Write into sqlite database
with open(args.csvfile, mode='r') as csvfile:

    con = None
    con = sqlite3.connect(args.db)

    if con is None:
        raise IOError('Unable to open sqlite database')

    cur = con.cursor()
    cur.execute(
        "CREATE TABLE IF NOT EXISTS " +
        "samples(id INTEGER PRIMARY KEY,{} INTEGER)".format(
                                        ' INTEGER, '.join(data_columns)))

    sql_insert = "INSERT INTO samples VALUES(NULL,{})".format(
                                        ','.join(['?']*len(data_columns)))

    reader = csv.DictReader(csvfile)

    for row in reader:

        line = []
        for key in data_columns:
            val = row[key]

            if key == 'timestamp':
                val = time.mktime(
                    datetime.datetime.strptime(
                        val, '%Y-%m-%d %H:%M:%S').timetuple())
            elif key == 'temperature' or key == 'humidity':
                val = int(float(val) * 100)

            line.append(val)

        cur.execute(sql_insert, line)

    con.commit()
