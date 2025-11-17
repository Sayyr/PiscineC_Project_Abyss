#include "game.h"
#include "input.h"
#include "pathfinding.h" // pour Node, MAX_PATH, astar
#include "state/state_explo.h"
#include "state/state_fight.h"
#include "state/state_hub.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void hub_enter(Game* g);
void hub_leave(Game* g);
void hub_update(Game* g, float dt);
void hub_render(Game* g);

void explo_enter(Game* g);
void explo_leave(Game* g);
void explo_update(Game* g, float dt);
void explo_render(Game* g);

void fight_enter(Game* g);
void fight_leave(Game* g);
void fight_update(Game* g, float dt);
void fight_render(Game* g);

static void game_apply_state_change(Game* g) {
    if (g->next_state == g->state) return;

    // 1) quitter l’état courant
    switch (g->state) {
        case GS_HUB:
            if (g->hub) {
                hub_leave(g);
                // leave devrait free sinon brutforce :
                // free(g->hub);
                g->hub = NULL;
            }
            break;
        case GS_EXPLORATION:
            if (g->explo) {
                explo_leave(g);
                g->explo = NULL;
            }
            break;
        case GS_COMBAT:
            if (g->fight) {
                fight_leave(g);
                g->fight = NULL;
            }
            break;
    }

    // 2) basculer
    g->state = g->next_state;

    // 3) entrer dans le nouvel état
    switch (g->state) {
        case GS_HUB:         hub_enter(g);   break;
        case GS_EXPLORATION: explo_enter(g); break;
        case GS_COMBAT:      fight_enter(g); break;
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

void game_shutdown(Game* g) {
    if (g->ren) SDL_DestroyRenderer(g->ren);
    if (g->win) SDL_DestroyWindow(g->win);
    SDL_Quit();
}

