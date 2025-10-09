#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "map.h"
#include "input.h" // doit exister et définir 'typedef struct Input Input;'

// Player défini avec tag (évite les conflits avec forward-decls)
typedef struct Player {
    float x, y, vx, vy;
    int hp;
} Player;

// include enemy.h après que Map et Player soient connus
#include "enemy.h"

typedef struct {
    Map map;
    Player player;
    Enemy enemy;
    // ...autres champs...
} Game;

void game_init(Game* g);
void game_update(Game* g, const Input* in);
void game_shutdown(Game* g);
