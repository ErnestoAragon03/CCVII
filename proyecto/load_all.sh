#!/bin/bash

echo "Cargando OS..."
echo "loady 0x80000000" > /dev/ttyUSB0
sleep 1
sx -vv os/os.bin < /dev/ttyUSB0 > /dev/ttyUSB0

echo "Cargando P1..."
echo "loady 0x80010000" > /dev/ttyUSB0
sleep 1
sx -vv p1/p1.bin < /dev/ttyUSB0 > /dev/ttyUSB0

echo "Cargando P2..."
echo "loady 0x80020000" > /dev/ttyUSB0
sleep 1
sx -vv p2/p2.bin < /dev/ttyUSB0 > /dev/ttyUSB0

# Opcional: arrancar el sistema operativo
echo "go 0x80000000" > /dev/ttyUSB0
