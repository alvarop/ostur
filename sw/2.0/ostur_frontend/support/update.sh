#!/bin/bash

echo "Updating files"
sudo rsync --chown=www-data:www-data -a `git rev-parse --show-toplevel`/sw/2.0/ostur_frontend /var/flaskapp/
sudo rsync --chown=www-data:www-data -a `git rev-parse --show-toplevel`/sw/2.0/ostur /var/flaskapp/

cd `git rev-parse --show-toplevel`/sw/2.0/ostur_frontend

echo "Updating nginx config setup"
sudo rm -f /etc/nginx/sites-enabled/default
sudo cp support/nginx_ostur_frontend /etc/nginx/sites-enabled/ostur_frontend

sudo rm -f /etc/nginx/nginx.conf
sudo cp support/nginx.conf /etc/nginx/nginx.conf

echo "Updating supervisor config setup"
sudo rm -f /etc/supervisor/conf.d/ostur_frontend.conf
sudo cp support/ostur_frontend.conf /etc/supervisor/conf.d/ostur_frontend.conf

cd -

echo "restart nginx"
sudo systemctl restart nginx

echo "restart supervisor"
sudo systemctl restart supervisor
