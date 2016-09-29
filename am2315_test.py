#!/usr/bin/env python
#

from silta import stm32f407
from AM2315 import AM2315
from MCP970x import MCP9701
import time

bridge = stm32f407.bridge('/dev/cu.usbmodem141421')

start_time = int(time.time())

thsensor = AM2315(bridge)

t1 = MCP9701(bridge, 'PA0')
t2 = MCP9701(bridge, 'PA1')

print('time,humidity,t0,t1,t2')

while True:
    humidity, temperature = thsensor.read()
    print (str(int(time.time()) - start_time) + ',' + 
            str(humidity) + ',' + str(temperature) + ',' +
            str(t1.read()) + ',' + str(t2.read()))

    time.sleep(1)

