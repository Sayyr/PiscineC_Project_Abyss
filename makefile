CC = gcc
SRC = main.c game.c input.c render.c pathfinding.c
EXE = crawler.exe

# À ADAPTER selon ton installation SDL2 :
SDL2_INCLUDE = C:/SDL2/include
SDL2_LIB = C:/SDL2/lib

CFLAGS = -Iinclude
LDFLAGS = -Llib -lSDL2main -lSDL2

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	del /Q *.exe

run: all
	./$(EXE)