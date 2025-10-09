#include "game.h"
#include "input.h"
#include "enemy.h"
#include "pathfinding.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#define MAX_PATH 64

static const char TEST_MAP[] =
"####################"
"#...............#..#"
"#..######.......#..#"
"#..#....#..........#"
"#..#....#######....#"
"#..................#"
"####################";

void game_init(Game* g) {
    g->map.w = 20; g->map.h = 7;
    g->map.data = TEST_MAP;
    g->player.x = 64; g->player.y = 64;
    g->player.vx = g->player.vy = 0;
    g->player.hp = 5;
    enemy_init(&g->enemy, 160, 160); // Position de départ de l’ennemi
}

bool map_is_wall(const Map* m, int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= m->w || ty >= m->h) return true;
    char c = m->data[ty * m->w + tx];
    return (c == '#');
}

// Teste la ligne de vue entre (x0, y0) et (x1, y1) sur la map (algorithme de Bresenham)
bool has_line_of_sight(const Map* map, float x0, float y0, float x1, float y1) {
    int steps = (int)(fmaxf(fabsf(x1 - x0), fabsf(y1 - y0)) / 4.0f);
    for (int i = 1; i <= steps; ++i) {
        float t = (float)i / steps;
        float x = x0 + (x1 - x0) * t;
        float y = y0 + (y1 - y0) * t;
        int tx = (int)(x / 32);
        int ty = (int)(y / 32);
        if (map_is_wall(map, tx, ty)) return false;
    }
    return true;
}

void game_update(Game* g, const Input* in) {
    const float speed = 150.0f; // px/s (tu peux ajouter un vrai dt plus tard)
    float ax = 0, ay = 0;
    if (in->left)  ax -= speed;
    if (in->right) ax += speed;
    if (in->up)    ay -= speed;
    if (in->down)  ay += speed;

    float nx = g->player.x + ax * (1.0f/60.0f);
    float ny = g->player.y + ay * (1.0f/60.0f);

    // collision “tile-based” simple (tile = 32px)
    int tx = (int)(nx / 32.0f);
    int ty = (int)(g->player.y / 32.0f);
    if (!map_is_wall(&g->map, tx, ty)) g->player.x = nx;

    tx = (int)(g->player.x / 32.0f);
    ty = (int)(ny / 32.0f);
    if (!map_is_wall(&g->map, tx, ty)) g->player.y = ny;

    // --- Déplacement ennemi avec collision ---
    float dx = g->player.x - g->enemy.x;
    float dy = g->player.y - g->enemy.y;
    float dist = sqrtf(dx*dx + dy*dy);
    float detection_radius = 128.0f;

    // Teste la ligne de vue
    if (dist < detection_radius && has_line_of_sight(&g->map, g->enemy.x, g->enemy.y, g->player.x, g->player.y)) {
        // Met à jour la dernière position connue
        g->enemy.last_player_x = g->player.x;
        g->enemy.last_player_y = g->player.y;
        g->enemy.has_last_player_pos = 1;
        // Pathfinding sur la grille
        int sx = (int)(g->enemy.x / 32), sy = (int)(g->enemy.y / 32);
        int ex = (int)(g->player.x / 32), ey = (int)(g->player.y / 32);
        Node path[MAX_PATH];
        int path_len = astar(&g->map, sx, sy, ex, ey, path);
        if (path_len > 0) {
            // Avance vers la première case du chemin
            float tx = path[0].x * 32 + 16;
            float ty = path[0].y * 32 + 16;
            float pdx = tx - g->enemy.x, pdy = ty - g->enemy.y;
            float plen = sqrtf(pdx*pdx + pdy*pdy);
            float speed = 60.0f * (1.0f/60.0f);
            if (plen > 1.0f) {
                g->enemy.x += (pdx/plen) * speed;
                g->enemy.y += (pdy/plen) * speed;
            }
        }
    } else if (g->enemy.has_last_player_pos) {
        float dx = g->enemy.last_player_x - g->enemy.x;
        float dy = g->enemy.last_player_y - g->enemy.y;
        float dist = sqrtf(dx*dx + dy*dy);
        if (dist > 4.0f) {
            // Pathfinding vers la dernière position connue
            int sx = (int)(g->enemy.x / 32), sy = (int)(g->enemy.y / 32);
            int ex = (int)(g->enemy.last_player_x / 32), ey = (int)(g->enemy.last_player_y / 32);
            Node path[MAX_PATH];
            int path_len = astar(&g->map, sx, sy, ex, ey, path);
            if (path_len > 0) {
                float tx = path[0].x * 32 + 16;
                float ty = path[0].y * 32 + 16;
                float pdx = tx - g->enemy.x, pdy = ty - g->enemy.y;
                float plen = sqrtf(pdx*pdx + pdy*pdy);
                float speed = 60.0f * (1.0f/60.0f);
                if (plen > 1.0f) {
                    g->enemy.x += (pdx/plen) * speed;
                    g->enemy.y += (pdy/plen) * speed;
                }
            }
        } else {
            // Arrivé à la dernière position connue
            g->enemy.has_last_player_pos = 0;
        }
    }

    // Collision joueur/ennemi
    float collision_dist = 24.0f;
    if (dist < collision_dist) {
        printf("attaque!\n");
    }
}

void game_shutdown(Game* g) {
    (void)g; // rien pour l’instant
}
