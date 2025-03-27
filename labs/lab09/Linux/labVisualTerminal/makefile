CC = gcc
CFLAGS = -lSDL2 -lGL -lGLU -lpthread
SRC = parking_terminal.c
OUT = terminal

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)

clean:
	rm -f $(OUT)

run: $(OUT)
	./$(OUT)
