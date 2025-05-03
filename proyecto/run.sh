#!/bin/bash

echo "🧹 Limpiando..."
rm -f *.o *.elf *.bin *.list

echo "⚙️  Ensamblando startup.s..."
arm-none-eabi-as startup.s -o startup.o

echo "🧠 Compilando OS..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -nostartfiles -ffreestanding -Wall os.c -o os.o

echo "👤 Compilando APP..."
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -nostartfiles -ffreestanding -Wall app.c -o app.o

echo "🔗 Enlazando todo con memmap..."
arm-none-eabi-ld -T memmap startup.o os.o app.o -o system.elf

echo "📦 Generando binario..."
arm-none-eabi-objcopy system.elf -O binary system.bin

echo "📜 Desensamblando..."
arm-none-eabi-objdump -D system.elf > system.list

echo "✅ ¡Compilación completada!"
ls -lh system.elf system.bin system.list
