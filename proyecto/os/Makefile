TARGET = os

SRCS = startup.s os.c timer.c pcb.c uart.c scheduler.c utils.c waitingList.c
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)  # convierte startup.s en startup.o

P1_OBJ = ../p1/p1.o
P2_OBJ = ../p2/p2.o
P3_OBJ = ../p3/p3.o

CFLAGS = -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard \
	-Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

LDFLAGS = -T ../linker/memmap_os.ld

# 🔧 Regla principal
all: libos.a $(TARGET).bin

# 🔨 Compilación de ensamblador
startup.o: startup.s
	arm-none-eabi-as --warn --fatal-warnings $< -o $@

# 🔨 Compilación de C
%.o: %.c
	arm-none-eabi-gcc -c $(CFLAGS) $< -o $@

# 📦 Librería estática
libos.a: $(OBJS)
	arm-none-eabi-ar rcs $@ $^

# 🧱 ELF incluyendo p1.o y p2.o
$(TARGET).elf: $(OBJS) $(P1_OBJ) $(P2_OBJ) $(P3_OBJ) 
	arm-none-eabi-ld $(LDFLAGS) $^ -o $@

# 📦 Binario final
$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

# 🧹 Limpieza
clean:
	rm -f *.o *.elf *.bin libos.a
