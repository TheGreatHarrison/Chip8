CC = gcc
CFLAGS = -Wextra -pedantic -std=c11 -I/opt/homebrew/Cellar/sdl2/2.28.1/include/ -I./lib

LDFLAGS = -L /opt/homebrew/Cellar/sdl2/2.28.1/lib -lSDL2

SRC = main.c display.c cpu.c instructions.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = chip8

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE) $(OBJ)
