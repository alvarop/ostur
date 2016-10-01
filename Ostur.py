'''
 AM2315 Temp/Humidity Sensor Driver (using Silta bridge)
 Based on the Adafruite Arduino driver
 https://github.com/adafruit/Adafruit_AM2315
'''

from silta import stm32f407
from AM2315 import AM2315
from MCP970x import MCP9701
import sys
import time

FRIDGE_PIN = 'PD0'
HUMIDIFIER_PIN = 'PD1'
WATER_PUMP_PIN = 'PD2'

class Ostur:
    def __init__(self, bridge_file):
        self.bridge = stm32f407.bridge(bridge_file)

        self.thsensor = AM2315(self.bridge)

        self.t1 = MCP9701(self.bridge, 'PA0')
        self.t2 = MCP9701(self.bridge, 'PA1')

        self.bridge.gpiocfg(FRIDGE_PIN, 'output')
        self.bridge.gpiocfg(HUMIDIFIER_PIN, 'output')
        self.bridge.gpiocfg(WATER_PUMP_PIN, 'output')

    def read_sensors(self):
        sensors = {}

        humidity0, temp0 = self.thsensor.read()
        temp1 = self.t1.read()
        temp2 = self.t2.read()

        sensors['time'] = int(time.time())
        sensors['humidity'] = humidity0
        sensors['temp0'] = temp0
        sensors['temp1'] = temp1
        sensors['temp2'] = temp2

        return sensors

    def enable_humidifier(self, state=False):
        if state is True:
            self.bridge.gpio(HUMIDIFIER_PIN, 1)
        else:
            self.bridge.gpio(HUMIDIFIER_PIN, 0)

