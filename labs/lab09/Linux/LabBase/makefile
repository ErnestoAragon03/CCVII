CC = gcc
CFLAGS = -Wall -pthread
TARGET = main

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET) -lrt

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)