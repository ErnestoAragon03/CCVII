TARGET = p3

SRCS = p3.c ../os/startup_process.s ../os/uart.c
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

CFLAGS = -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard \
	-Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

LDFLAGS = -T ../linker/memmap_p3.ld

all: $(TARGET).bin

# Compilar .c a .o
%.o: %.c
	arm-none-eabi-gcc -c $(CFLAGS) $< -o $@

# Compilar .s a .o
%.o: %.s
	arm-none-eabi-as --warn --fatal-warnings $< -o $@

# Linkear .o a .elf
$(TARGET).elf: $(OBJS)
	arm-none-eabi-ld $(LDFLAGS) $^ -o $@

# Convertir .elf a .bin
$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

clean:
	rm -f *.o *.elf *.bin
