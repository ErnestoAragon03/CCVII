#!/bin/bash

echo "⚙️ Compilando Prog1..."
FLAGS="-mcpu=cortex-a8 -mfpu=neon -mfloat-abi=soft -nostdlib -nostartfiles -ffreestanding -Wall"

arm-none-eabi-as minimal_start.s -o start.o
arm-none-eabi-gcc -c $FLAGS p1.c -o p1.o
arm-none-eabi-gcc -c $FLAGS uart.c -o uart.o

echo "🔗 Enlazando Prog1 con prog1.ld..."
arm-none-eabi-ld -T prog1.ld start.o p1.o uart.o -o prog1.elf
arm-none-eabi-objcopy prog1.elf -O binary Prog1.bin

echo "✅ Prog1.bin generado."
