# Instrucciones para compilar
1. Clean Previous Builds
  - rm -f *.o hello.elf hello.bin hello.list
2. Compile c program
  - arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -
nostdlib -nostartfiles -Zreestanding hello.c -o hello.o
3. Assemble startup.s
  - arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o
5. Link the object files
  - arm-none-eabi-ld -T memmap startup.o hello.o -o hello.elf
6. Generate Binary File
  - arm-none-eabi-objcopy hello.elf -O binary hello.bin
7. Dissamble the ELF File (Optional) Disassemble the ELF file to verify addresses 
  - arm-none-eabi-objdump -D hello.elf > hello.list

# Configuración del puerto serial
- Configure Your Terminal Program with the following settings:
- Baud Rate: 115200
- Data Bits: 8
- Parity: None
- Stop Bits: 1
- Flow Control: None
- Open the Serial Connection to the BeagleBone Black. You should be able to see
U-Boot messages when the board is powered on
