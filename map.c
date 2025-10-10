#include "map.h"
#include <math.h>

bool map_is_wall(const Map* m, int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= m->w || ty >= m->h) return true;
    char c = m->data[ty * m->w + tx];
    return (c == '#');
}

// Convertit positions en pixels en coordonnées de tuiles (une tuiiiile = 32 px).
bool has_line_of_sight(const Map* map, float x0f, float y0f, float x1f, float y1f) {
    const int TILE = 32;
    int x0 = (int)floorf(x0f / TILE);
    int y0 = (int)floorf(y0f / TILE);
    int x1 = (int)floorf(x1f / TILE);
    int y1 = (int)floorf(y1f / TILE);

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    int x = x0, y = y0;
    while (1) {
        if (map_is_wall(map, x, y)) return false;
        if (x == x1 && y == y1) break;
        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 <  dx) { err += dx; y += sy; }
    }
    return true;
}