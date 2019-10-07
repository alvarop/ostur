#!/bin/bash

pipenv run python -u ostur_logger.py --port /dev/ttyUSB0 --baud_rate 115200 --db /home/pi/ostur.db
