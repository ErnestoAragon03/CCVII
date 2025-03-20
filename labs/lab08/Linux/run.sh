#!/bin/bash

# Compilar el programa
make clean
make

# Ejecutar el programa
./main

# Mostrar el contenido del log
echo "Contenido del archivo de log:"
cat transactions.log
