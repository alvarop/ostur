'''
 Ostur Cheese Cave Controller Logger
'''

import serial
import sys
import time
from datetime import datetime


class InvalidRelayException(Exception):
    pass


class Ostur:

    DEBUG = False

    # Hardcoding until we can read values back from device
    __CMD_MAX_STR_LEN = 1024

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

        # Flush any remaining data in the input buffer
        self.__send_cmd('\n')
        self.stream.reset_input_buffer()

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
        self.stream.reset_input_buffer()
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
        line = self.stream.readline()

        result = line.strip().split(' ')
        if result[0] == 'DATA':
            # Return samples without DATA and the last comma
            line = (result[1].strip()[:-1]).split(',')

            # Temporary workaround so it supports both kinds of timestamps
            try:
                # Convert YYYYMMDDTHHMMSS to unix timestamp in milliseconds
                dt = datetime.strptime(line[0], '%Y%m%dT%H%M%S')
                line[0] = int(time.mktime(dt.timetuple())) * 1000
            except ValueError:
                pass

            return line

        return line

    def set_localtime(self):
        timecmd = time.strftime("time %Y %m %d %H %M %S", time.localtime())
        self.__send_cmd(timecmd)

    def get_time(self):
        line = self.__send_cmd('time')

        result = line.strip().split(' ')

        if result[0] == 'OK':
            # Convert YYYYMMDDTHHMMSS to unix timestamp in milliseconds
            dt = datetime.strptime(result[1], '%Y%m%dT%H%M%S')
            return int(time.mktime(dt.timetuple())) * 1000
        else:
            return None
