CC = gcc
CFLAGS = -Wextra -pedantic -std=c11 -I/opt/homebrew/Cellar/sdl2/2.28.1/include/ -I/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include/ -I./inc/

LDFLAGS = -L /opt/homebrew/Cellar/sdl2/2.28.1/lib -lSDL2

SRC = src/main.c src/display.c src/cpu.c src/instructions.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = chip8

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE) $(OBJ)
