#!/bin/bash

echo "Updating files"
sudo rsync --chown=www-data:www-data -a `git rev-parse --show-toplevel`/sw/2.0/serial_packet /var/
sudo rsync --chown=www-data:www-data -a `git rev-parse --show-toplevel`/sw/2.0/ostur /var/

sudo pip3 install -e /var/ostur
sudo pip3 install -e /var/serial_packet

cd `git rev-parse --show-toplevel`/sw/2.0/ostur_logger

echo "Updating supervisor config setup"
sudo rm -f /etc/supervisor/conf.d/ostur_logger.conf
sudo cp support/ostur_logger.conf /etc/supervisor/conf.d/ostur_logger.conf

cd -

echo "restart supervisor"
sudo systemctl restart supervisor
