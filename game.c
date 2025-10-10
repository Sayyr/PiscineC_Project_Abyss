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

void game_init(Game* game) {
    game->map.w = 20; game->map.h = 7;
    game->map.data = TEST_MAP;
    game->player.x = 64; game->player.y = 64;
    game->player.vx = game->player.vy = 0;
    game->player.hp = 5;
    enemy_init(&game->enemy, 160, 160);
}

void game_update(Game* game, const Input* in) {
    const float speed = 150.0f; // px/s (tu peux ajouter un vrai dt plus tard)
    float ax = 0, ay = 0;
    if (in->left)  ax -= speed;
    if (in->right) ax += speed;
    if (in->up)    ay -= speed;
    if (in->down)  ay += speed;

    float nx = game->player.x + ax * (1.0f/60.0f);
    float ny = game->player.y + ay * (1.0f/60.0f);

    // collision tile-based (tile = 32px)
    int tx = (int)(nx / 32.0f);
    int ty = (int)(game->player.y / 32.0f);
    if (!map_is_wall(&game->map, tx, ty)) game->player.x = nx;

    tx = (int)(game->player.x / 32.0f);
    ty = (int)(ny / 32.0f);
    if (!map_is_wall(&game->map, tx, ty)) game->player.y = ny;

    // Update ennemi via enemy.c
    enemy_update(&game->enemy, &game->map, &game->player);

    // Collision joueur/ennemi gérée dans enemy_update (ou ici si tu préfères)
}

void game_shutdown(Game* game) {
    (void)game;
}
