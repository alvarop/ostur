#!/usr/bin/env python
#
# Script to log/control temperature using HTTP requests to control_server
# Basic threshold control at the moment, PID to come later
# 

import urllib, json
import sys
import time
import numpy as np
import sqlite3

DEFAULT_SETTINGS = {
    'NUM_SAMPLES': 10,       # Number of samples to average 
    'TEMP_SETTING': 11,      # 'goal' temperature
    'LOW_THRESHOLD': 0,      # Low temp threshold
    'HIGH_THRESHOLD': 0.25,  # High temp threshold
}

class OsturController:
    def __init__(self, control_server, db_name=None):
        
        self.settings = DEFAULT_SETTINGS
        self.control_server = control_server

        self.temps = [0] * self.settings['NUM_SAMPLES']

        if db_name is None:
            db_name = time.strftime('ostur_control_log_%Y-%m-%d_%H-%M-%S.db', time.localtime())

        self.con = None
        self.con = sqlite3.connect(db_name)

        if self.con is None:
            raise IOError('Unable to open sqlite database')

        self.cur = self.con.cursor()
        self.cur.execute("CREATE TABLE IF NOT EXISTS samples(id INTEGER PRIMARY KEY, timestamp INTEGER, t1 INTEGER, h1 INTEGER, t2 INTEGER, t3 INTEGER)")
        self.cur.execute("CREATE TABLE IF NOT EXISTS events(id INTEGER PRIMARY KEY, timestamp INTEGER, description TEXT)")

        try:
            info = self.ostur_request('/info')
            print('Connected!')
            print(info)

        except IOError:
            print('Unable to connect to control server')

    def ostur_request(self, request_path):
        response = urllib.urlopen(self.control_server + request_path)
        return json.loads(response.read())

    def fridge_control(self, control_server, enable=False):
        request_path = '/relay/fridge/'
        if enable is True:
            request_path += 'enable'
        else:
            request_path += 'disable'

        success = self.ostur_request(request_path)
        # TODO - check response and do something about it...
        
        print success

    def control(self, current_temp):
        # Remove oldest sample
        self.temps.pop(0)
        self.temps.append(current_temp)
        temp = np.mean(self.temps)

        print self.temps, temp

        if temp > self.settings['TEMP_SETTING'] + self.settings['HIGH_THRESHOLD']:
            self.fridge_control(control_server, True)
        elif temp < self.settings['TEMP_SETTING'] - self.settings['LOW_THRESHOLD']:
            self.fridge_control(control_server, False)

    def run(self):
        while True:
            try:
                response = urllib.urlopen(control_server + '/sensors')
                sensors = json.loads(response.read())

                self.control(sensors['temp0'])

                line = []

                # Saving integer data to reduce SQLite db size
                line.append(int(sensors['time']))

                # Storing units * 10 to keep 1 significant figure
                line.append(int(sensors['humidity'] * 10))
                line.append(int(sensors['temp0'] * 10))
                line.append(int(sensors['temp1'] * 10))
                line.append(int(sensors['temp2'] * 10))

                self.cur.execute("INSERT INTO samples VALUES(NULL,?,?,?,?,?)", line)
                self.con.commit()
                print line

            except ValueError:
                print('# Error decoding JSON')

            time.sleep(1)



control_server = 'http://' + sys.argv[1]

controller = OsturController(control_server)

controller.run()
