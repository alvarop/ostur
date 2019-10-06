#!/bin/bash

export PIPENV_VENV_IN_PROJECT=true
export DATABASE=/home/pi/ostur.db
pipenv run gunicorn --workers 5 --bind unix:ostur_frontend.sock -m 007 src:app
