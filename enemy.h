#pragma once
#include <SDL2/SDL.h>
typedef struct Map Map;
typedef struct Player Player;
#include "game.h"
#include "pathfinding.h"

// Prototype de structure Enemy (pour plugins/futurs types dynamiques)
typedef struct Enemy Enemy;

typedef struct EnemyVTable {
    void (*update)(Enemy* self, const Map* map, const Player* player);
    void (*render)(Enemy* self, SDL_Renderer* ren);
    // Ajoute ici d'autres callbacks si besoin (ex: on_attack, on_death, etc.)
} EnemyVTable;

// Structure de base d'un ennemi
struct Enemy {
    float x, y;
    int hp;
    float last_player_x, last_player_y;
    int has_last_player_pos;
    int wander_target_x, wander_target_y;
    const EnemyVTable* vtable; // Pour le système de plugins
    // Ajoute ici des champs spécifiques ou un void* pour données plugin
};

// Fonctions de base
void enemy_init(Enemy* e, float x, float y);
void enemy_update(Enemy* e, const Map* map, const Player* player);
void enemy_render(Enemy* e, SDL_Renderer* ren);
void enemy_wander(Enemy* e, const Map* map);