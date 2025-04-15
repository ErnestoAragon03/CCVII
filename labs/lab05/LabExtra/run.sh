#!/bin/bash

# Limpiar anteriores
rm -f *.o *.elf *.bin *.list

# Compilar archivo C
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -O2 -ffreestanding process_states_baremetal.c -o process.o

# Compilar startup.s (asumiendo que ya existe en el mismo directorio)
arm-none-eabi-as startup.s -o startup.o

# Linkear todo con script memmap
arm-none-eabi-ld -T memmap -o process.elf startup.o process.o

# Generar archivo binario
arm-none-eabi-objcopy -O binary process.elf process.bin

# (Opcional) Generar disassembly
arm-none-eabi-objdump -D process.elf > process.list

