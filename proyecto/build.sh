#!/bin/bash


echo "⚙️  Ensamblando startup.s..."
arm-none-eabi-as startup.s -o startup.o

FLAGS="-mcpu=cortex-a8 -mfpu=neon -mfloat-abi=soft -nostdlib -nostartfiles -ffreestanding -Wall"

echo "🧠 Compilando fuentes..."
arm-none-eabi-gcc -c $FLAGS utils.c -o utils.o
arm-none-eabi-gcc -c $FLAGS uart.c -o uart.o
arm-none-eabi-gcc -c $FLAGS pcb.c -o pcb.o
arm-none-eabi-gcc -c $FLAGS context.c -o context.o
arm-none-eabi-gcc -c $FLAGS timer.c -o timer.o
arm-none-eabi-gcc -c $FLAGS scheduler.c -o scheduler.o
arm-none-eabi-gcc -c $FLAGS os.c -o os.o
arm-none-eabi-gcc -c $FLAGS p1.c -o p1.o
arm-none-eabi-gcc -c $FLAGS p2.c -o p2.o

echo "🔗 Enlazando con memmap..."
arm-none-eabi-ld -T memmap startup.o os.o utils.o uart.o pcb.o context.o timer.o scheduler.o p1.o p2.o -o system.elf

echo "📦 Generando binario..."
arm-none-eabi-objcopy system.elf -O binary system.bin

echo "📜 Desensamblando..."
arm-none-eabi-objdump -D system.elf > system.list

echo "✅ ¡Compilación completada!"
ls -lh system.elf system.bin system.list
