#Un archivo makefile su proposito
#decidir qué partes de un programa grande deben volver a compilarse
CC=gcc
CFLAGS=-Wall -pthread
TARGET=file_processor

all: $(TARGET)

$(TARGET): main_linux.c
	$(CC) $(CFLAGS) -o $(TARGET) main_linux.c

clean:
	rm -f $(TARGET)