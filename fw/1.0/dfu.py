#!/usr/bin/python
# Script to set device in DFU mode and flash with new firmware image
# Example usage: dfu.py --filename controller.bin --dev /dev/ttyACM0

import serial
import sys
import time
import argparse
import os

VID = 0x0483
PID = 0xdf11

FLASH_ADDR = 0x8000000

parser = argparse.ArgumentParser()

parser.add_argument('--filename',
                    required=True,
                    help='Binary file to flash')
parser.add_argument('--dev',
                    required=True,
                    help='Usb device to connect to')

args = parser.parse_args()

dfu_cmd = 'dfu-util'
dfu_cmd += ' -d {:04X}:{:04X}'.format(VID, PID)
dfu_cmd += ' -c 1 -i 0 -a 0'
dfu_cmd += ' -s 0x{:08X}:leave'.format(FLASH_ADDR)
dfu_cmd += ' -D {}'.format(args.filename)

stream = serial.Serial()
stream.port = args.dev
stream.timeout = 0.1
stream.open()

stream.write('\n')
print("Sending DFU command")
stream.write('dfu\n')

print("Wait 2 seconds for device to enumerate")
time.sleep(2)

print("Downloading firmware")
os.system(dfu_cmd)

print("Done!")
