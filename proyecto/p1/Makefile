# p1/Makefile

TARGET = p1

all: $(TARGET).bin

$(TARGET).o: $(TARGET).c
	arm-none-eabi-gcc -c -mcpu=cortex-a8 -Wall -Werror -O2 \
	-nostdlib -nostartfiles -ffreestanding $< -o $@

$(TARGET).elf: $(TARGET).o
	arm-none-eabi-ld -T memmap_p1.ld $< -o $@

$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

clean:
	rm -f *.o *.elf *.bin
