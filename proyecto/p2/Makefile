TARGET = p2

# 🟢 PRIMER objetivo, debe estar arriba
all: $(TARGET).bin

CFLAGS = -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard \
	-Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

$(TARGET).o: $(TARGET).c
	arm-none-eabi-gcc -c $(CFLAGS) $< -o $@

# Compila p1.o si no está hecho
../p1/p1.o:
	$(MAKE) -C ../p1 p1.o

# Enlaza el .elf con p1.o y libos.a
$(TARGET).elf: $(TARGET).o ../os/libos.a ../p1/p1.o
	arm-none-eabi-ld -T ../linker/memmap_p2.ld $^ -o $@

# Genera el .bin
$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

# Limpieza
clean:
	rm -f *.o *.elf *.bin
