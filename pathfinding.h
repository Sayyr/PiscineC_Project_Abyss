#pragma once
#include "map.h"
typedef struct Map Map;

#define MAX_PATH 256

typedef struct { int x, y; } Node;

int astar(const Map* map, int sx, int sy, int ex, int ey, Node path[MAX_PATH]);
