#!/bin/bash

# Assemble startup.s
arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o

# Compile hello.c
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding hello.c -o hello.o

# Link with memmap.ld
arm-none-eabi-ld -T memmap.ld startup.o hello.o -o program.elf

# Generate binary
arm-none-eabi-objcopy -O binary program.elf program.bin

echo "Build complete!"
