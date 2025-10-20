#include "render_explo.h"
#include "../state_explo.h"   // pour connaître la struct Explo

void render_explo(SDL_Renderer* r, const struct Explo* s) {
    // fond
    SDL_SetRenderDrawColor(r, 8, 8, 10, 255);
    SDL_RenderClear(r);

    // map 32px
    for (int y=0; y<s->map.h; ++y) {
        for (int x=0; x<s->map.w; ++x) {
            char c = s->map.data[y*s->map.w + x];
            if (c == '#') {
                SDL_Rect t = { x*32, y*32, 32, 32 };
                SDL_SetRenderDrawColor(r, 40, 44, 52, 255);
                SDL_RenderFillRect(r, &t);
            }
        }
    }

    // joueur
    SDL_Rect p = { (int)s->player.x - 10, (int)s->player.y - 10, 20, 20 };
    SDL_SetRenderDrawColor(r, 240, 220, 80, 255);
    SDL_RenderFillRect(r, &p);
}
