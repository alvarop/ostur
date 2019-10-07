#!/bin/bash

cd `git rev-parse --show-toplevel`/sw/2.0/ostur_logger

echo "Updating supervisor config setup"
sudo rm -f /etc/supervisor/conf.d/ostur_logger.conf
sudo cp support/ostur_logger.conf /etc/supervisor/conf.d/ostur_logger.conf

cd -

echo "restart supervisor"
sudo systemctl restart supervisor
