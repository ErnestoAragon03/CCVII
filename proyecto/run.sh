#!/bin/bash

set -e  # Terminar si hay errores

echo "Compilando ensamblador (root.s)..."
arm-none-eabi-as -o root.o root.s

echo "Compilando núcleo del sistema (os.c)..."
arm-none-eabi-gcc -c -O0 -nostdlib -ffreestanding -o os.o os.c

echo "Compilando funciones de E/S (stdio.c)..."
arm-none-eabi-gcc -c -O0 -nostdlib -ffreestanding -o stdio.o stdio.c

echo "Compilando utilidades (utils.c)..."
arm-none-eabi-gcc -c -O0 -nostdlib -ffreestanding -o utils.o utils.c

echo "Compilando proceso 1 (process1.c)..."
arm-none-eabi-gcc -c -O0 -nostdlib -ffreestanding -o process1.o process1.c

echo "Compilando proceso 2 (process2.c)..."
arm-none-eabi-gcc -c -O0 -nostdlib -ffreestanding -o process2.o process2.c

echo "Enlazando todo con linker.ld..."
arm-none-eabi-ld -T linker.ld -o kernel.elf root.o os.o stdio.o utils.o process1.o process2.o

echo "Generando binario final (kernel.bin)..."
arm-none-eabi-objcopy -O binary kernel.elf kernel.bin

echo "Compilación completada. Puedes transferir kernel.bin a la BeagleBone."
