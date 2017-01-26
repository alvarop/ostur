'''
 AM2315 Temp/Humidity Sensor Driver (using Silta bridge)
 Based on the Adafruite Arduino driver
 https://github.com/adafruit/Adafruit_AM2315
'''

import serial
from am2315 import AM2315
from mcp970x import MCP9701
import sys
import time


class InvalidRelayException(Exception):
    pass

class Ostur:

    DEBUG = False

    # Hardcoding until we can read values back from device
    __CMD_MAX_STR_LEN = 1024
    __SPI_MAX_BYTES = 512
    __I2C_MAX_BYTES = 512

    def __init__(self, serial_device, baud_rate=None):
        self.stream = None

        try:
            self.stream = serial.Serial()
            self.stream.port = serial_device
            self.stream.timeout = 0.1
            if baud_rate:
                self.stream.baudrate = baud_rate
            self.stream.open()
        except OSError:
            raise IOError('could not open ' + serial_device)

        if self.stream:
            self.stream.flush()

        # Flush any remaining data in the silta's buffer
        self.__send_cmd('\n')

        # Get device serial number and save it
        line = self.__send_cmd('sn\n')
        result = line.strip().split(' ')

        if result[0] == 'OK':
            self.serial_number = ''.join(result[1:])
        else:
            self.serial_number = None
            print('Warning: Could not read device serial number.')
            print('You might want to update firmware on your board')

        # Get device serial number and save it
        line = self.__send_cmd('version\n')
        result = line.strip().split(' ')

        if result[0] == 'OK':
            self.firmware_version = result[1]
        else:
            self.firmware_version = None
            print('Warning: Could not read device firmware version.')
            print('You might want to update firmware on your board')

    def close(self):
        ''' Disconnect from USB-serial device. '''
        self.stream.close()

    # Send terminal command and wait for response
    def __send_cmd(self, cmd):

        if (len(cmd) + 1) > self.__CMD_MAX_STR_LEN:
            raise RuntimeException('Command string too long')

        self.stream.write(cmd + '\n')
        if self.DEBUG is True:
            print 'CMD : ' + cmd

        line = self.stream.readline()
        if self.DEBUG is True:
            print 'RESP: ' + line,

        return line

    def start_sampling(self):
        self.stream.flush()
        line = self.__send_cmd('controller start')
        self.stream.timeout = 20  # This should be dependent on sample rate

        result = line.strip().split(' ')
        if result[0] == 'DATA':
            # Return header without DATA and the last comma
            return (result[1].strip()[:-1]).split(',')
        else:
            return result

    def stop_sampling(self):
        self.__send_cmd('controller stop')
        self.stream.timeout = 0.1

    def read_sensors(self):
        sensors = {}
        line = self.stream.readline()

        result = line.strip().split(' ')
        if result[0] == 'DATA':
            # Return samples without DATA and the last comma
            return (result[1].strip()[:-1]).split(',')

        return sensors
