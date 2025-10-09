#include "game.h"
#include "input.h"
#include "pathfinding.h" // pour Node, MAX_PATH, astar
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
    enemy_init(&g->enemy, 160, 160);
}

bool map_is_wall(const Map* m, int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= m->w || ty >= m->h) return true;
    char c = m->data[ty * m->w + tx];
    return (c == '#');
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

    // collision tile-based (tile = 32px)
    int tx = (int)(nx / 32.0f);
    int ty = (int)(g->player.y / 32.0f);
    if (!map_is_wall(&g->map, tx, ty)) g->player.x = nx;

    tx = (int)(g->player.x / 32.0f);
    ty = (int)(ny / 32.0f);
    if (!map_is_wall(&g->map, tx, ty)) g->player.y = ny;

    // Update ennemi via enemy.c
    enemy_update(&g->enemy, &g->map, &g->player);

    // Collision joueur/ennemi gérée dans enemy_update (ou ici si tu préfères)
}

void game_shutdown(Game* g) {
    (void)g;
}
