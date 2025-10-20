#include "game.h"
#include "input.h"
#include "pathfinding.h" // pour Node, MAX_PATH, astar
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

