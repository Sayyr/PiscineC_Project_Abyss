#include "map.h"

bool map_is_wall(const Map* m, int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= m->w || ty >= m->h) return true;
    char c = m->data[ty * m->w + tx];
    return (c == '#');
}