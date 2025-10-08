#include "render.h"

void render_frame(SDL_Renderer* r, const Game* g) {
    SDL_SetRenderDrawColor(r, 18, 18, 22, 255);
    SDL_RenderClear(r);

    // Dessine la map (32px la tuile)
    for (int y = 0; y < g->map.h; ++y) {
        for (int x = 0; x < g->map.w; ++x) {
            char c = g->map.data[y * g->map.w + x];
            if (c == '#') {
                SDL_Rect tile = { x*32, y*32, 32, 32 };
                SDL_SetRenderDrawColor(r, 60, 60, 70, 255);
                SDL_RenderFillRect(r, &tile);
            }
        }
    }

    // Joueur (carré 24x24)
    SDL_Rect p = { (int)g->player.x - 12, (int)g->player.y - 12, 24, 24 };
    SDL_SetRenderDrawColor(r, 220, 200, 60, 255);
    SDL_RenderFillRect(r, &p);

    SDL_RenderPresent(r);
}
