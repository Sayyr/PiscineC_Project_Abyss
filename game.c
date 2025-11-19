#include "game.h"
#include "input.h"
#include "pathfinding.h" // pour Node, MAX_PATH, astar
#include "state/state_explo.h"
#include "state/state_fight.h"
#include "state/state_hub.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void game_apply_state_change(Game* g) {
    if (g->next_state == g->state) return;

    GameState old = g->state;
    GameState new = g->next_state;

    // 1) quitter l’état courant
    switch (old) {
        case GS_HUB:
            if (g->hub) {
                hub_leave(g, &g->hub);
                // leave devrait free sinon brutforce :
                g->hub = NULL;
            }
            break;
        case GS_EXPLORATION:
        
            if (g->explo) {
                explo_leave(g, &g->explo);
                g->explo = NULL;
            }
            break;
        case GS_COMBAT:
            if (new != GS_COMBAT && g->explo) {
                explo_leave(g, &g->explo);
                // g->explo = NULL;
            }
            break;
    }

    // 2) basculer
    g->state = new;

    // 3) entrer dans le nouvel état
    switch (g->state) {
        case GS_HUB:         hub_enter  (g, &g->hub)  ; break;
        case GS_EXPLORATION: explo_enter(g, &g->explo); break;
        case GS_COMBAT:      fight_enter(g, &g->fight); break;
    }
}

bool game_init(Game* g, const char* title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) return false;
    g->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    if (!g->win) return false;
    g->ren = SDL_CreateRenderer(g->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g->ren) return false;
    g->running   = true;
    g->state     = GS_HUB;
    g->next_state= GS_HUB;
    g->hub = NULL; g->explo = NULL; g->fight = NULL; // alloués par les enter()
    return true;
}

void game_step(Game* g, float dt){
    switch (g->state)
    {
    case GS_HUB:
        hub_update(g, g->hub, dt);
        break;
    case GS_EXPLORATION:
        explo_update(g, g->explo, dt);
        break;
    case GS_COMBAT:
        fight_update(g, g->fight, dt);
        break;    
    }
    game_apply_state_change(g);
}

void game_shutdown(Game* g) {
    if (g->ren) SDL_DestroyRenderer(g->ren);
    if (g->win) SDL_DestroyWindow(g->win);
    SDL_Quit();
}

