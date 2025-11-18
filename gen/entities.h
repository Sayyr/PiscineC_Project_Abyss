#pragma once
#include <include/SDL2/SDL.h>
#include "map.h"
#include "player.h"
#include "../state/render/render_common.h"
#include "../include/creatures.h"

typedef enum { WANDER, TRAQUE, SEARCH } EnemyState;

typedef struct EnemyVTable EnemyVTable;
typedef struct Enemy {
    float x, y;
    int   hp;
    EnemyState state;
    int dead;

    // mémoire de cible
    float last_player_x, last_player_y;
    int   has_last_player_pos;

    // wander
    int   wander_target_x, wander_target_y;

    const EnemyVTable* vtable;
    Inventory inv;

    MarineCreature creature;
} Enemy;

struct EnemyVTable {
    void (*update)(Enemy*, const Map*, const Player*);
    void (*render)(Enemy*, SDL_Renderer*);
};

typedef struct Chest {
    Inventory inv;
    int opened;
} Chest;

void enemy_init_from_creature(Enemy* e, float x, float y, const MarineCreature* c);
void enemy_update(Enemy* e, const Map* map, const Player* player);
void enemy_render(Enemy* e, SDL_Renderer* ren);
void enemy_wander(Enemy* e, const Map* map);
