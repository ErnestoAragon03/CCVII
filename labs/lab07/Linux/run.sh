#!/bin/bash




LOG_FILE="access.log"
EXECUTABLE="./log_analyzer"

if [ ! -f "$EXECUTABLE" ]; then
    echo "Compilando el programa..."
    make
fi

if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: No se pudo compilar el programa."
    exit 1
fi

if [ ! -f "$LOG_FILE" ]; then
    echo "Error: Archivo de log '$LOG_FILE' no encontrado."
    exit 1
fi

# Ejecutar el programa
$EXECUTABLE $LOG_FILE
