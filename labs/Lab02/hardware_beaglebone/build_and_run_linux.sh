#!/bin/bash

# Exit on error
set -e

# Cleanup
echo "Cleaning previous build..."
rm -f *.o *.a calculadora.elf calculadora.bin

# Assemble startup
echo "Assembling root.s..."
arm-none-eabi-as -o root.o root.s

# Compile sources
echo "Compiling os.c..."
arm-none-eabi-gcc -mcpu=cortex-a8 -mfloat-abi=soft -c os.c -o os.o
arm-none-eabi-ar rcs os.a os.o

echo "Compiling utils.c..."
arm-none-eabi-gcc -mcpu=cortex-a8 -mfloat-abi=soft -c utils.c -o utils.o
arm-none-eabi-ar rcs utils.a utils.o

echo "Compiling stdio.c..."
arm-none-eabi-gcc -mcpu=cortex-a8 -mfloat-abi=soft -c stdio.c -o stdio.o
arm-none-eabi-ar rcs stdio.a stdio.o

echo "Compiling main.c..."
arm-none-eabi-gcc -mcpu=cortex-a8 -mfloat-abi=soft -c main.c -o main.o

# Link
echo "Linking..."
arm-none-eabi-gcc -mcpu=cortex-a8 -mfloat-abi=soft -nostartfiles -T linker.ld \
  -o calculadora.elf root.o main.o stdio.a os.a utils.a -lc -lm

# Convert to bin
echo "Generating binary..."
arm-none-eabi-objcopy -O binary calculadora.elf calculadora.bin

echo "✅ Build complete. Binary ready: calculadora.bin"
