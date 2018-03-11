#!/usr/bin/env python
"""
Script used BLED112 usb dongle to receive Ostur packets over BLE.
The received data is printed out and optionally saved to a csv file.

The data can then be plotted by plot_data.py
"""

import argparse
import serial
import struct
import time
import datetime
import os.path
import sqlite3
import json
import base64
from bglib import bglib

last_index = -1
last_ch = -1

eddystone_types = {
    0x00: 'uid',
    0x10: 'url',
    0x20: 'tlm',
    0x30: 'eid'
}


def decode_eddystone_data(data):
    """ Basic eddystone packet decoder
    Used data from:
    https://github.com/google/eddystone/blob/master/protocol-specification.md
    """
    if len(data) < 4:
        return None

    data_len, data_type, uuid = struct.unpack_from('<BBH', data, 0)

    if data_len != 3:
        return None

    if data_type != 3:
        return None

    if uuid != 0xFEAA:
        return None

    data_len, data_type, uuid = struct.unpack_from('<BBH', data, 4)

    if data_type != 0x16:
        return None

    if uuid != 0xFEAA:
        return None

    eddystone_type = data[8]

    if eddystone_type not in eddystone_types:
        return {'type': 'unknown', 'data': data[8:(8+data_len)]}

    if eddystone_types[eddystone_type] == 'uid':
        return {'type': 'uid', 'uid': data[10:26]}

    return {'type': eddystone_types[eddystone_type], 'uid': data[8:8+data_len]}


def process_ostur_packet(packet, rssi):
    global last_index, last_ch

    (magic,
        device_id,
        sample_index,
        sensor_ch,
        fridge_state,
        temperature,
        humidity,
        flags) = \
        struct.unpack('<HIHBBhhH', packet)

    if magic != 0x0578:
        return

    if sample_index != last_index or sensor_ch != last_ch:
        last_index = sample_index
        last_ch = sensor_ch

        data = {}
        data['sample_index'] = sample_index
        data['timestamp'] = int(time.time())
        data['fridge_state'] = fridge_state
        data['temperature'] = temperature
        data['humidity'] = humidity
        data['sensor_ch'] = sensor_ch
        data['device_id'] = device_id
        data['flags'] = flags

        if args.csvfile:
            save_csv_data(data)

        if args.db:
            save_sqlite_data(data)

        print_data(data)


data_columns = ['timestamp', 'device_id', 'sample_index', 'sensor_ch',
                'temperature', 'humidity', 'fridge_state', 'flags']


sql_insert = "INSERT INTO samples VALUES(NULL,{})".format(
                                    ','.join(['?']*len(data_columns)))


def save_sqlite_data(data):
    line = []
    for key in data_columns:
        line.append(data[key])
    cur.execute(sql_insert, line)
    con.commit()


def save_csv_data(data):
    line = ''

    for key in data_columns:
        if key == 'timestamp':
            line += datetime.datetime.fromtimestamp(
                data['timestamp']).strftime('%Y-%m-%d %H:%M:%S')
        elif key == 'temperature' or key == 'humidity':
            line += str(data[key]/100.0)
        else:
            line += str(data[key])

        line += ','

    csvfile.write(line + '\n')
    csvfile.flush()


def write_csv_header():
    header = ''
    for item in data_columns:
        header += str(item) + ','
    csvfile.write(header + '\n')


def print_data(data):
    line = ''

    for key in data_columns:
        if key == 'timestamp':
            line += datetime.datetime.fromtimestamp(
                data['timestamp']).strftime('%Y-%m-%d %H:%M:%S')
        elif key == 'temperature' or key == 'humidity':
            line += str(data[key]/100.0)
        else:
            line += str(data[key])
        line += ','

    print(line)


def to_hex_str(data):
    hex_str = ''
    for byte in data:
        hex_str += '{:02X}'.format(byte)
    return hex_str


def timeout(sender, args):
    raise IOError('BGLib timeut :(')


def encode_json(data):
    b64_data = {}
    for key, val in data.items():
        if type(val) is bytes:
            b64_data[key + '_base64'] = base64.b64encode(val).decode('utf-8')
        else:
            b64_data[key] = val

    return json.dumps(b64_data)


def decode_json(json_data):
    data = {}

    base64_data = json.loads(json_data)

    for key, val in base64_data.items():
        if '_base64' in key:
            original_key = key[:-len('_base64')]
            data[original_key] = base64.b64decode(val)
        else:
            data[key] = val

    return data


def process_scan_response(sender, args):
    # json_args = encode_json(args)
    # print(json_args)

    eddystone_data = decode_eddystone_data(args['data'])
    if eddystone_data and eddystone_data['type'] == 'uid':
        process_ostur_packet(eddystone_data['uid'], args['rssi'])


parser = argparse.ArgumentParser()

parser.add_argument('--baud_rate',
                    default=115200,
                    type=int,
                    help='BLED112 baud rate')

parser.add_argument('--port',
                    required=True,
                    help='BLED112 device to connect to')

parser.add_argument('--csvfile',
                    help='Output csvfile (csv format)')

parser.add_argument('--db',
                    help='Sqlite db csvfile')

args = parser.parse_args()

if os.path.isfile(args.csvfile):
    need_header = False
else:
    need_header = True

if args.csvfile:
    csvfile = open(args.csvfile, mode='a')

    if need_header:
        write_csv_header()

if args.db:
    con = None
    con = sqlite3.connect(args.db)

    if con is None:
        raise IOError('Unable to open sqlite database')

    cur = con.cursor()
    cur.execute(
        "CREATE TABLE IF NOT EXISTS " +
        "samples(id INTEGER PRIMARY KEY,{} INTEGER)".format(
                                        ' INTEGER, '.join(data_columns)))


ble = bglib.BGLib()
ble.packet_mode = False

ble.on_timeout += timeout

ble.ble_evt_gap_scan_response += process_scan_response

ser = serial.Serial(port=args.port, baudrate=args.baud_rate, timeout=1)
ser.flushInput()
ser.flushOutput()

# disconnect if we are connected already
ble.send_command(ser, ble.ble_cmd_connection_disconnect(0))
ble.check_activity(ser, 1)

# stop advertising if we are advertising already
ble.send_command(ser, ble.ble_cmd_gap_set_mode(0, 0))
ble.check_activity(ser, 1)

# stop scanning if we are scanning already
ble.send_command(ser, ble.ble_cmd_gap_end_procedure())
ble.check_activity(ser, 1)

# set scan parameters
ble.send_command(ser, ble.ble_cmd_gap_set_scan_parameters(0xC8, 0xC8, 1))
ble.check_activity(ser, 1)

# start scanning now
ble.send_command(ser, ble.ble_cmd_gap_discover(2))
ble.check_activity(ser, 1)

while (1):
    # check for all incoming data (no timeout, non-blocking)
    ble.check_activity(ser)

    # don't burden the CPU
    time.sleep(0.01)
