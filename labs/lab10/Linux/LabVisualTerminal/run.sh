#!/bin/bash

# Compilar
if make; then
    # Ejecutar si la compilación fue exitosa
    ./zombie_bridge
else
    echo "Error en la compilación"
    exit 1
fi