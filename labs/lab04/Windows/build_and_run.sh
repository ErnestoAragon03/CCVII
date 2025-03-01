#!/bin/bash

# Limpiar archivos antiguos
echo "Limpiando archivos viejos..."
rm -f t1.exe t2.exe t3.exe t4.exe t5.exe

# Compilar todos los programas
echo "🛠  Compilando programas..."
gcc tarea1.c -o t1 && echo "✅ tarea1.c compilado"
#gcc tarea2.c -o t2 && echo "✅ tarea2.c compilado"
#gcc tarea3.c -o t3 && echo "✅ tarea3.c compilado"
#gcc tarea4.c -o t4 && echo "✅ tarea4.c compilado"
#gcc tarea5.c -o t5 && echo "✅ tarea5.c compilado"

echo -e "\nEjecutando laboratorio..."
echo "=============================="

echo -e "\Ejecutando Tarea 1"
./t1

echo -e "\nTodas las tareas completadas!"