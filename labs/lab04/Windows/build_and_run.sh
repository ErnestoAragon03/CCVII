#!/bin/bash

# Limpiar archivos antiguos
echo "Limpiando archivos viejos..."
rm -f *.exe

# Compilar todos los programas
echo "🛠  Compilando programas..."
gcc tarea1.c -o t1 && echo "✅ tarea1.c compilado"
gcc tarea2.c -o t2 && echo "✅ tarea2.c compilado"
gcc tarea3.c -o t3 && echo "✅ tarea3.c compilado"
gcc tarea4.c -o t4 && echo "✅ tarea4.c compilado"
gcc tarea5.c -o t5 && echo "✅ tarea5.c compilado"
gcc child_process.c -o child_process && echo "✅ child_process.c compilado"
gcc child_process_3.c -o child_process_3 && echo "✅ child_process_3.c compilado"
gcc child_process_5.c -o child_process_5 && echo "✅ child_process_5.c compilado"

echo -e "\nEjecutando laboratorio..."
echo "=============================="

for i in {1..5}; do
    echo -e "\nEjecutando Tarea $i"
    ./t$i
    sleep 1
done

echo -e "\n=============================="
echo -e "Todas las tareas completadas!"