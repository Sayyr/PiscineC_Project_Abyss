#pragma once
#include <stdbool.h>

typedef struct Map {
    int w;
    int h;
    const char* data;   // '#' mur, '.' sol, etc.
} Map;

bool map_is_wall(const Map* m, int tx, int ty);
bool has_line_of_sight(const Map* map, float x0, float y0, float x1, float y1);
