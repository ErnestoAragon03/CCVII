TARGET = p1

# 🟢 PRIMER objetivo, debe estar arriba
all: $(TARGET).bin

CFLAGS = -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard \
	-Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

$(TARGET).o: $(TARGET).c
	arm-none-eabi-gcc -c $(CFLAGS) $< -o $@

# Compila p2.o si no está hecho
../p2/p2.o:
	$(MAKE) -C ../p2 p2.o

# Enlaza el .elf con p2.o y libos.a
$(TARGET).elf: $(TARGET).o ../os/libos.a ../p2/p2.o
	arm-none-eabi-ld -T ../linker/memmap_p1.ld $^ -o $@

# Genera el .bin
$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

# Limpieza
clean:
	rm -f *.o *.elf *.bin
