CC = gcc
CFLAGS = -Wall -pthread

all: log_analyzer

log_analyzer: main_linux.o log_processor.o
	$(CC) $(CFLAGS) -o log_analyzer main_linux.o log_processor.o

main_linux.o: main_linux.c log_processor.h
	$(CC) $(CFLAGS) -c main_linux.c

log_processor.o: log_processor.c log_processor.h
	$(CC) $(CFLAGS) -c log_processor.c

clean:
	rm -f *.o log_analyzer

python:
	rm -f access.log && python3 access_log_file_generator.py
