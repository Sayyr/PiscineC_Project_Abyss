#include "render.h"

void draw_circle(SDL_Renderer* ren, int cx, int cy, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(ren, cx + dx, cy + dy);
            }
        }
    }
}

void render_frame(SDL_Renderer* r, const Game* game) {
    SDL_SetRenderDrawColor(r, 18, 18, 22, 255);
    SDL_RenderClear(r);

    // Dessine la map (32px la tuile)
    for (int y = 0; y < game->map.h; ++y) {
        for (int x = 0; x < game->map.w; ++x) {
            char c = game->map.data[y * game->map.w + x];
            if (c == '#') {
                SDL_Rect tile = { x*32, y*32, 32, 32 };
                SDL_SetRenderDrawColor(r, 60, 60, 70, 255);
                SDL_RenderFillRect(r, &tile);
            }
        }
    }

    // Joueur (carré 24x24)
    SDL_Rect p = { (int)game->player.x - 12, (int)game->player.y - 12, 24, 24 };
    SDL_SetRenderDrawColor(r, 220, 200, 60, 255);
    SDL_RenderFillRect(r, &p);

    // Dessine l’ennemi en rouge
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    draw_circle(r, (int)game->enemy.x, (int)game->enemy.y, 12);

    SDL_RenderPresent(r);
}
