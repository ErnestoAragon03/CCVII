# Clean
rm -f *.o *.elf *.bin

# Assemble startup.s
arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o

# Compile os.c
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding os.c -o os.o

# Compile p1.c
arm-none-eabi-gcc -c -mcpu=cortex-a8 -nostdlib -nostartfiles -ffreestanding -Wall p1.c -o p1.o

# Link with memmap.ld
arm-none-eabi-ld -T memmap startup.o os.o p1.o -o system.elf

# Generate binary
arm-none-eabi-objcopy -O binary system.elf system.bin

echo "Build complete!"
