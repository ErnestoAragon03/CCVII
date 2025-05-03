#!/bin/bash

echo "🧹 Limpiando archivos anteriores..."
rm -f *.o p1.elf p1.bin p1.list

echo "⚙️  Ensamblando startup.s..."
arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o 

echo "🧠 Compilando p1.c..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding p1.c -o p1.o 


echo "🔗 Enlazando con memmap..."
arm-none-eabi-ld -T memmap startup.o p1.o -o p1.elf 


echo "📦 Generando p1.bin..."
arm-none-eabi-objcopy p1.elf -O binary p1.bin 


echo "🔍 Desensamblando a p1.list (opcional)..."
arm-none-eabi-objdump -D p1.elf > p1.list

echo "✅ Compilación completa. Archivos generados:"
ls -lh p1.elf p1.bin p1.list
