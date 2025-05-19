#!/bin/bash

echo "⚙️ Compilando OS..."
arm-none-eabi-as startup.s -o startup.o

FLAGS="-mcpu=cortex-a8 -mfpu=neon -mfloat-abi=soft -nostdlib -nostartfiles -ffreestanding -Wall"

# Compilar cada fuente por separado
arm-none-eabi-gcc -c $FLAGS os.c -o os.o
arm-none-eabi-gcc -c $FLAGS uart.c -o uart.o
arm-none-eabi-gcc -c $FLAGS timer.c -o timer.o
arm-none-eabi-gcc -c $FLAGS context.c -o context.o
arm-none-eabi-gcc -c $FLAGS scheduler.c -o scheduler.o
arm-none-eabi-gcc -c $FLAGS pcb.c -o pcb.o
arm-none-eabi-gcc -c $FLAGS utils.c -o utils.o

echo "🔗 Enlazando OS con os.ld..."
arm-none-eabi-ld -T os.ld startup.o os.o uart.o timer.o context.o scheduler.o pcb.o utils.o -o os.elf

echo "📦 Generando OS.bin..."
arm-none-eabi-objcopy os.elf -O binary OS.bin

echo "✅ OS.bin generado."
