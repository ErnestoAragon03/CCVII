#!/bin/bash

echo "🧹 Limpiando archivos anteriores..."
rm -f *.o hello.elf hello.bin hello.list

echo "⚙️  Ensamblando startup.s..."
arm-none-eabi-as startup.s -o startup.o || { echo "❌ Error en ensamblado"; exit 1; }

echo "🧠 Compilando hello.c..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard \
  -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding hello.c -o hello.o \
  || { echo "❌ Error en compilación"; exit 1; }

echo "🔗 Enlazando con memmap..."
arm-none-eabi-ld -T memmap startup.o hello.o -o hello.elf \
  || { echo "❌ Error en enlace"; exit 1; }

echo "📦 Generando hello.bin..."
arm-none-eabi-objcopy hello.elf -O binary hello.bin \
  || { echo "❌ Error al generar binario"; exit 1; }

echo "🔍 Desensamblando a hello.list (opcional)..."
arm-none-eabi-objdump -D hello.elf > hello.list

echo "✅ Compilación completa. Archivos generados:"
ls -lh hello.elf hello.bin hello.list
