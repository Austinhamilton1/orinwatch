#!/bin/bash

set -e

# Build
mkdir -p build
cd build
cmake ..
make

# Install binary and systemd unit
sudo make install

# Reload systemd and enable/start service
sudo systemctl daemon-reexec
sudo systemctl daemon-reload
sudo systemctl enable orinwatch
sudo systemctl start orinwatch

echo "OrinWatch has been installed and started."
