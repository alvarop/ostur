#!/bin/bash

# Make sure we're in the right directory
cd `git rev-parse --show-toplevel`/sw/2.0/ostur_logger

echo "Remove old setup"
sudo rm -r /var/ostur
sudo rm -r /var/serial_packet

bash ./support/update.sh

cd -
