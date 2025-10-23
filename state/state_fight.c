#include "state_fight.h"
#include <../../include/SDL2/SDL.h>
#include <stdlib.h>

void fight_enter(Game* g, Fight** ps) {
    (void)g;
    if (!*ps) *ps = (Fight*)calloc(1, sizeof(Fight));
    (*ps)->round = 1;
}
void fight_leave(Game* g, Fight** ps) {
    (void)g;
    free(*ps); *ps = NULL;
}
void fight_update(Game* g, Fight* s, float dt) {
    (void)s; (void)dt;
    const Uint8* ks = SDL_GetKeyboardState(NULL);
    // Debug: V = victoire (retour explo), L = défaite (retour hub)
    if (ks[SDL_SCANCODE_V]) game_change_state(g, GS_EXPLORATION);
    if (ks[SDL_SCANCODE_L]) game_change_state(g, GS_HUB);
}
void fight_render(Game* g, Fight* s) {
    (void)s;
    SDL_SetRenderDrawColor(g->ren, 20, 10, 10, 255);
    SDL_RenderClear(g->ren);
    // (dessiner UI de combat plus tard)
}
