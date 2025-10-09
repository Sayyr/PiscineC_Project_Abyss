#pragma once
#include <stdbool.h>

typedef struct Map {
    int w;
    int h;
    const char* data;
} Map;

bool map_is_wall(const Map* m, int tx, int ty);