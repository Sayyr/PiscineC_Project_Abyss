#pragma once
#include "game.h"
#include "../map.h"
#include "gen/entities.h"
#include "player/player.h"

// Mini map+player pour la démo
// typedef struct {
//     int w, h;
//     const char* data; // '#' mur, '.' sol
// } Map;

// typedef struct {
//     float x, y;
// } DemoPlayer;

typedef struct Explo {
    Map    map;
    Player player;
    Enemy      ennemies[4];
    int        enemy_count;
} Explo;

void explo_enter (Game* g, Explo** ps);
void explo_update(Game* g, Explo*  s, float dt);
void explo_render(Game* g, Explo*  s);
void explo_leave (Game* g, Explo** ps);
