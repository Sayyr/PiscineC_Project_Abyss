#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "map.h"
#include "input.h" // doit exister et définir 'typedef struct Input Input;'
typedef struct Player {
    float x, y, vx, vy;
    int hp;
} Player;

#include "enemy.h"

typedef struct {
    Map map;
    Player player;
    Enemy enemy;
    // ...autres champs...
} Game;

void game_init(Game* game);
void game_update(Game* game, const Input* in);
void game_shutdown(Game* game);
