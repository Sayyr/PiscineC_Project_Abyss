#pragma once
#include "game.h"

// Mini map+player pour la démo (tu peux brancher ton map.h existant)
typedef struct {
    int w, h;
    const char* data; // '#' mur, '.' sol
} DemoMap;

typedef struct {
    float x, y;
} DemoPlayer;

typedef struct Explo {
    DemoMap    map;
    DemoPlayer player;
} Explo;

void explo_enter (Game* g, Explo** ps);
void explo_update(Game* g, Explo*  s, float dt);
void explo_render(Game* g, Explo*  s);
void explo_leave (Game* g, Explo** ps);
