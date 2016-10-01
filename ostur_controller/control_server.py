#!/usr/bin/python 
from flask import Flask, jsonify
from Ostur import Ostur
import sys

ostur = Ostur(sys.argv[1])
app = Flask(__name__)

@app.route('/')
@app.route('/info')
def info():
	info = {}
	info['silta_fw'] = ostur.bridge.firmware_version
	return jsonify(info)

@app.route('/sensors')
def sensors():
	sensors = ostur.read_sensors()
	return jsonify(sensors)

@app.route('/humidifier/<state>')
def humidifier_on(state):
	
	response = {}

	if state == 'enable':
		ostur.enable_humidifier(True)
		response['success'] = True
	elif state == 'disable':
		ostur.enable_humidifier(False)
		response['success'] = True
	else:
		print('unknown state!')
		response['success'] = False

	return jsonify(response)


if __name__ == '__main__':
	
	app.run(debug=True, host='0.0.0.0')
	