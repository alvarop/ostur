#!/usr/bin/env python
"""
Script uses nrf52811 usb dongle to receive Ostur packets over BLE.
The received data is printed out and optionally saved to a sqlite file.
"""

import argparse
import os
import serial
import sys
import yaml
import time
from datetime import datetime
from ostur.packets import BLEOsturPacket
from serial_packet.serial_packet import decode_packet, encode_packet
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

parser = argparse.ArgumentParser()

parser.add_argument("config",  help="config file")
parser.add_argument("port", help="serial device to connect to")

args = parser.parse_args()

with open(args.config, "r") as cfg_yml:
    cfg = yaml.safe_load(cfg_yml)

stream = serial.Serial(args.port, baudrate=115200, timeout=0.01)
stream.flushInput()

last_packet = {}

client = InfluxDBClient(url=cfg["server_url"], token=cfg["token"])
write_api = client.write_api(write_options=SYNCHRONOUS)

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
        packet_dict["uid"] in last_packet
        and last_packet[packet_dict["uid"]]["sample"] == packet_dict["sample"]
    ):
        return

    last_packet[packet_dict["uid"]] = packet_dict

    data = {}
    for key, value in packet_dict.items():
        if key == "temperature" or key == "humidity":
            data[key] = value / 100.0
        elif key == "battery":
            data[key] = value / 1000.0
        else:
            data[key] = packet_dict[key]
    data = BLEOsturPacket.from_dict(data)

    influx_data = f"ostur,uid={data.uid} battery={data.battery},humidity={data.humidity},temperature={data.temperature},rssi={data.rssi},sample={data.sample}"
    print(influx_data)
    write_api.write(cfg["bucket"], cfg["org"], influx_data)


buff = bytearray()

while True:
    line = stream.read(1)
    if len(line) > 0:
        buff.append(line[0])
        while decode_packet(buff, process_packet) is True:
            pass
