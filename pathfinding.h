#pragma once
#include "game.h"
typedef struct Map Map;
typedef struct Player Player;
#define MAX_PATH 64

typedef struct { int x, y; } Node;

int astar(const Map* map, int sx, int sy, int ex, int ey, Node path[MAX_PATH]);