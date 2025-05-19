#!/bin/bash

echo "⚙️ Compilando Prog2..."
FLAGS="-mcpu=cortex-a8 -mfpu=neon -mfloat-abi=soft -nostdlib -nostartfiles -ffreestanding -Wall"

arm-none-eabi-as minimal_start.s -o start.o
arm-none-eabi-gcc -c $FLAGS p2.c -o p2.o
arm-none-eabi-gcc -c $FLAGS uart.c -o uart.o

echo "🔗 Enlazando Prog2 con prog2.ld..."
arm-none-eabi-ld -T prog2.ld start.o p2.o uart.o -o prog2.elf
arm-none-eabi-objcopy prog2.elf -O binary Prog2.bin

echo "✅ Prog2.bin generado."
