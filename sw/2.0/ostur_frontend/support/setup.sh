#!/bin/bash

# Make sure we're in the right directory
cd `git rev-parse --show-toplevel`/sw/2.0/ostur_frontend

echo "Remove old setup"
sudo rm -r /var/flaskapp/ostur_frontend
sudo rm -r /var/ostur

bash ./support/update.sh

cd -

