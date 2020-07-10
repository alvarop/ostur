#!/usr/bin/env python
"""
Script uses nrf52811 usb dongle to receive Ostur packets over BLE.
The received data is printed out and optionally saved to a sqlite file.
"""

import argparse
import os
import serial
import sys
import time
from datetime import datetime
from ostur.packets import BLEOsturPacket
from ostur.osturdb import OsturDB
from serial_packet.serial_packet import decode_packet, encode_packet


parser = argparse.ArgumentParser()

parser.add_argument("--baud_rate", default=115200, type=int, help="baud rate")
parser.add_argument("--port", required=True, help="device to connect to")
parser.add_argument("--db", help="Sqlite db file")

args = parser.parse_args()

if args.db:
    db = OsturDB(args.db)
else:
    db = None

stream = serial.Serial(args.port, baudrate=args.baud_rate, timeout=0.01)
stream.flushInput()

last_packet = None


def process_packet(packet):
    global last_packet

    packet_dict = BLEOsturPacket.decode(packet)._asdict()

    # Older fw transmits mfg data of size one and value 0x57
    if packet_dict["mfg_data_len"] == 1 and packet_dict["mfg_data"] & 0xFF == 0x57:
        pass
    # Newer fw just uses the magic field with no mfg_data
    elif packet_dict["mfg_data_len"] == 0 and packet_dict["magic"] == 0x5757:
        pass
    else:
        # Not a valid packet
        return

    # Don't process duplicate packets!
    if (
        last_packet is not None
        and last_packet["uid"] == packet_dict["uid"]
        and last_packet["sample"] == packet_dict["sample"]
    ):
        return

    last_packet = packet_dict

    data = {}
    for key, value in packet_dict.items():
        if key == "temperature" or key == "humidity":
            data[key] = value / 100.0
        elif key == "battery":
            data[key] = value / 1000.0
        else:
            data[key] = packet_dict[key]
    data = BLEOsturPacket.from_dict(data)

    print(data)
    if db is not None:
        db.add_record(data)


buff = bytearray()

while True:
    line = stream.read(1)
    if len(line) > 0:
        buff.append(line[0])
        while decode_packet(buff, process_packet) is True:
            pass
