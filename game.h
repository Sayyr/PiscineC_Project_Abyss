#pragma once
#include <include/SDL2/SDL.h>
#include <stdbool.h>

typedef enum { GS_HUB, GS_EXPLORATION, GS_COMBAT } GameState;

// Fwds des contextes d'états (définis dans leurs .h)
typedef struct Hub    Hub;
typedef struct Explo  Explo;
typedef struct Fight Fight;

typedef struct {
    SDL_Window*   win;
    SDL_Renderer* ren;
    bool          running;

    GameState state;      // état courant
    GameState next_state; // demande de transition (ou = state si rien)

    // Contextes d'états (possédés par Game, passés aux fonctions des états)
    Hub*    hub;
    Explo*  explo;
    Fight* fight;

    int combat_enemy_index;
} Game;

// Transition à demander depuis un état (jamais modifier Game.state directement)
static inline void game_change_state(Game* g, GameState ns) { g->next_state = ns; }

bool game_init(Game* g, const char* title, int w, int h);
void game_shutdown(Game* g);
void game_step(Game* g, float dt);