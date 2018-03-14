# Ostur Software

The ostur software consists of a few python scripts that listen in to the Ostur BLE beacons and log data.

## Requirements
1. [python 3+](https://www.python.org/) - All the scripts are in python
2. [pipenv](https://github.com/pypa/pipenv) - Pipenv helps with all the dependencies and makes sure you don't "pollute" your system with packages
3. A [Silicon Labs BLED112](https://www.digikey.com/products/en?keywords=1446-1030-ND) USB dongle for receiving BLE beacons

## Setup Instructions
1. On the top directory (of the entire repo), run `pipenv install --three`. This should set up a virtual environment for python and install all required libraries for the scripts to run.

## Capturing and plotting data
1. To capture Ostur beacon data `pipenv run sw/2.0/scripts/simple_scan.py --port /dev/ttyACM0 --csvfile ostur.csv`
  * /dev/ttyACM0 is the serial port of the BLED112 in my particular linux setup. This will be different on MacOS and Windows.
  * The data is saved into a csv file. The --db argument can be used instead to save it as an sqlite database instead
2. To plot the captured data (stored in a csv file) `pipenv run sw/2.0/scripts/plot_data.py --filename ostur.csv`
