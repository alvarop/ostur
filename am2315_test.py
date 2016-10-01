#!/usr/bin/env python
#

from Ostur import Ostur
import sys
import time

ostur = Ostur(sys.argv[1])

print('time,humidity,t0,t1,t2')

while True:
    sensors = ostur.read_sensors()

    line = []
    line.append(str(int(time.time())))
    line.append(str(sensors['humidity']))
    line.append(str(sensors['temp0']))
    line.append(str(sensors['temp1']))
    line.append(str(sensors['temp2']))
    print(','.join(line))

    time.sleep(1)
