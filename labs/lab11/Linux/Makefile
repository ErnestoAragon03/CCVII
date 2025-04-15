# Nombre del ejecutable
TARGET = scheduling

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -O2

# Archivos fuente
SRCS = scheduling.c

# Regla por defecto
all: $(TARGET)

# Cómo compilar
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Limpiar archivos generados
clean:
	rm -f $(TARGET)
