#!/bin/bash

# Compilamos el proyecto
make

# Si la compilación fue exitosa, ejecutamos el programa
if [ $? -eq 0 ]; then
    ./terminal
else
    echo "Hubo un error durante la compilación."
fi
