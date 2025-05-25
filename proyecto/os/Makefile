# os/Makefile

TARGET = os
OBJS = startup.o os.o

all: $(TARGET).bin

startup.o: startup.s
	arm-none-eabi-as --warn --fatal-warnings startup.s -o $@

os.o: os.c
	arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard \
	-Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding os.c -o $@

$(TARGET).elf: $(OBJS)
	arm-none-eabi-ld -T memmap_os.ld $(OBJS) -o $@

$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

clean:
	rm -f *.o *.elf *.bin
