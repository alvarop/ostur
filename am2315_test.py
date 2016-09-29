#!/usr/bin/env python
#

from silta import stm32f407
from am2315 import AM2315
import time

bridge = stm32f407.bridge('/dev/cu.usbmodem141421')

start_time = int(time.time())

thsensor = AM2315(bridge)

while True:
    humidity, temperature = thsensor.read()
    print str(int(time.time()) - start_time) + ',' + str(humidity) + ',' + str(temperature)
    time.sleep(1)

