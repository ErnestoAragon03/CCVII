#!/bin/bash

echo "🧹 Limpiando..."
rm -f *.o *.elf *.bin *.list
rm -f os/*.o prog1/*.o prog2/*.o

# Incluir rutas
INCLUDES="-Ios -Iprog1 -Iprog2"
CFLAGS="-mcpu=cortex-a8 -nostdlib -nostartfiles -ffreestanding -Wall"

echo "⚙️  Compilando OS..."
arm-none-eabi-as os/startup.s -o os/startup.o
arm-none-eabi-gcc -c $CFLAGS $INCLUDES os/os.c -o os/os.o
arm-none-eabi-gcc -c $CFLAGS $INCLUDES os/utils.c -o os/utils.o
arm-none-eabi-gcc -c $CFLAGS $INCLUDES os/uart.c -o os/uart.o
arm-none-eabi-gcc -c $CFLAGS $INCLUDES os/pcb.c -o os/pcb.o
arm-none-eabi-gcc -c $CFLAGS $INCLUDES os/waitingList.c -o os/waitingList.o
arm-none-eabi-gcc -c $CFLAGS $INCLUDES os/timer.c -o os/timer.o
arm-none-eabi-gcc -c $CFLAGS $INCLUDES os/scheduler.c -o os/scheduler.o

echo "🔗 Enlazando OS..."
arm-none-eabi-ld -T os/memmap_os.ld \
    os/startup.o os/os.o os/utils.o os/uart.o os/pcb.o os/waitingList.o os/timer.o os/scheduler.o \
    -o os.elf
arm-none-eabi-objcopy os.elf -O binary os.bin
arm-none-eabi-objdump -D os.elf > os.list

echo "👤 Compilando Programa 1..."
arm-none-eabi-gcc -c $CFLAGS $INCLUDES prog1/p1.c -o prog1/p1.o
arm-none-eabi-as prog1/minimal_start.s -o prog1/start.o
arm-none-eabi-ld -T prog1/memmap_prog1.ld \
    prog1/start.o prog1/p1.o os/uart.o os/utils.o \
    -o prog1.elf
arm-none-eabi-objcopy prog1.elf -O binary prog1.bin
arm-none-eabi-objdump -D prog1.elf > prog1.list

echo "👤 Compilando Programa 2..."
arm-none-eabi-gcc -c $CFLAGS $INCLUDES prog2/p2.c -o prog2/p2.o
arm-none-eabi-as prog2/minimal_start.s -o prog2/start.o
arm-none-eabi-ld -T prog2/memmap_prog2.ld \
    prog2/start.o prog2/p2.o os/uart.o os/utils.o \
    -o prog2.elf
arm-none-eabi-objcopy prog2.elf -O binary prog2.bin
arm-none-eabi-objdump -D prog2.elf > prog2.list

echo "✅ ¡Compilación completa!"
ls -lh os.bin prog1.bin prog2.bin
