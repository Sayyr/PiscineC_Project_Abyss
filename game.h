#pragma once
#include <stdbool.h>
#include "input.h"

typedef struct {
    float x, y;      // position du joueur
    float vx, vy;    // vitesse
    int   hp;
} Player;

typedef struct {
    int w, h;        // taille de la map en cases
    const char* data;// pointeur vers tuiles (mur '.', mur '#', etc.)
} Map;

typedef struct {
    Player player;
    Map    map;
} Game;

// typedef struct Input Input; // forward decl.

void game_init(Game* g);
void game_update(Game* g, const Input* in);
void game_shutdown(Game* g);

// Utils collisions
bool map_is_wall(const Map* m, int tx, int ty);
