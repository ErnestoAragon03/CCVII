#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Remove previous compiled objects and binaries
echo "Cleaning up previous build files..."
rm -f root.o main.o stdio.o stdio.a calculadora.elf calculadora.bin

# Assemble the startup code
echo "Assembling startup.s..."
arm-none-eabi-as -o root.o root.s

# Compile the libraries
echo "Compiling Language Libraries..."
arm-none-eabi-gcc -mcpu=arm926ej-s -mfloat-abi=soft -c stdio.c -o stdio.o
arm-none-eabi-ar rcs stdio.a stdio.o

# Compile the main application
echo "Compiling main.c..."
arm-none-eabi-gcc -mcpu=arm926ej-s -mfloat-abi=soft -c main.c -o main.o

# Link the object files
echo "Linking object files..."
arm-none-eabi-gcc -mcpu=arm926ej-s -mfloat-abi=soft -nostartfiles -T linker.ld -o calculadora.elf root.o main.o stdio.a -lc -lm

# Convert the ELF file to a binary
echo "Converting ELF to binary..."
arm-none-eabi-objcopy -O binary calculadora.elf calculadora.bin

# Run the binary in QEMU
echo "Running QEMU..."
qemu-system-arm -M versatilepb -nographic -kernel calculadora.elf
