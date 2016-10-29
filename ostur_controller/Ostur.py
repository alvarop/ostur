'''
 AM2315 Temp/Humidity Sensor Driver (using Silta bridge)
 Based on the Adafruite Arduino driver
 https://github.com/adafruit/Adafruit_AM2315
'''

from silta import stm32f407
from am2315 import AM2315
from mcp970x import MCP9701
import sys
import time

relays = {
    'fridge': {'pin':'PD0'},
    'humidifier': {'pin':'PD1'},
    'water_pump': {'pin':'PD2'}
}

class InvalidRelayException(Exception):
    pass

class Ostur:
    def __init__(self, bridge_file):
        self.bridge = stm32f407.bridge(bridge_file)

        self.relays = relays

        self.thsensor = AM2315(self.bridge)

        self.t1 = MCP9701(self.bridge, 'PA0')
        self.t2 = MCP9701(self.bridge, 'PA1')

        self.bridge.gpiocfg(self.relays['fridge']['pin'], 'output')
        self.bridge.gpiocfg(self.relays['humidifier']['pin'], 'output')
        self.bridge.gpiocfg(self.relays['water_pump']['pin'], 'output')

    def read_sensors(self):
        sensors = {}

        humidity0, temp0 = self.thsensor.read()
        temp1 = self.t1.read()
        temp2 = self.t2.read()

        sensors['time'] = time.time()
        sensors['humidity'] = humidity0
        sensors['temp0'] = temp0
        sensors['temp1'] = temp1
        sensors['temp2'] = temp2

        return sensors

    def relay_control(self, relay, state=False):
        if relay not in self.relays:
            raise InvalidRelayException('Invalid Relay ' + relay)

        if state is True:
            self.bridge.gpio(self.relays[relay]['pin'], 1)
        else:
            self.bridge.gpio(self.relays[relay]['pin'], 0)

    def relay_list(self):
        return sorted(self.relays.keys())

    def relay_status(self):
        status = {}
        for key in sorted(self.relays.keys()):
            status[key] = self.bridge.gpio(self.relays[key]['pin'])

        return status

