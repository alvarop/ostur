#!/bin/bash

# Make sure we're in the right directory
cd `git rev-parse --show-toplevel`/sw/2.0/ostur_logger

echo "pipenv setup"

pipenv install --three

bash ./support/update.sh

cd -
