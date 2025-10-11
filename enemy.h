#pragma once
#include <SDL2/SDL.h>
typedef struct Map Map;
typedef struct Player Player;
typedef struct Enemy Enemy;

typedef struct EnemyVTable {
    void (*update)(Enemy* self, const Map* map, const Player* player);
    void (*render)(Enemy* self, SDL_Renderer* ren);
} EnemyVTable;

typedef enum {WANDER, SEARCH, TRAQUE, STAY} EnemyState;
struct Enemy {
    float x, y;
    int hp;
    float last_player_x, last_player_y;
    int has_last_player_pos;
    int wander_target_x, wander_target_y;
    const EnemyVTable* vtable;
    EnemyState state;
};

void enemy_init(Enemy* e, float x, float y);
void enemy_update(Enemy* e, const Map* map, const Player* player);
void enemy_render(Enemy* e, SDL_Renderer* ren);
void enemy_wander(Enemy* e, const Map* map);