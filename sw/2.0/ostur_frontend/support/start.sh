#!/bin/bash

export DATABASE=/home/pi/ostur.db
gunicorn --workers 5 --bind unix:ostur_frontend.sock -m 007 src:app
