CC = gcc
CFLAGS = -Wextra -pedantic -std=c99 -I /opt/homebrew/Cellar/sdl2/2.28.1/include/

LDFLAGS = -L /opt/homebrew/Cellar/sdl2/2.28.1/lib -lSDL2

SRC = main.c display.c cpu.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = chip8

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE) $(OBJ)
