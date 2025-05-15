#!/bin/bash

echo "🧹 Limpiando archivos anteriores..."
rm -f *.o *.elf *.bin

echo "⚙️  Ensamblando startup.s..."
arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o

echo "🧠 Compilando hello.c (bare-metal)..."
arm-none-eabi-gcc -c \
  -mcpu=cortex-a8 \
  -mfpu=neon \
  -mfloat-abi=soft \
  -Wall -Werror -O2 \
  -nostdlib -nostartfiles -ffreestanding \
  hello.c -o hello.o

echo "🔗 Enlazando con memmap.ld..."
arm-none-eabi-ld -T memmap.ld startup.o hello.o -o program.elf

echo "📦 Generando archivo binario..."
arm-none-eabi-objcopy -O binary program.elf program.bin

echo "✅ ¡Compilación completada con éxito!"
ls -lh program.elf program.bin
