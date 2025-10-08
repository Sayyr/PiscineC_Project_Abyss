#include <SDL.h>
#include <stdbool.h>
#include "game.h"
#include "input.h"
#include "render.h"

int main(void) {
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    SDL_Window* win = SDL_CreateWindow("Crawler",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Game game = {0};
    game_init(&game);                 // État initial (player, map…)

    bool running = true;
    while (running) {
        Input in = input_read(&running);     // 1) événements > actions
        game_update(&game, &in);             // 2) logique
        render_frame(ren, &game);            // 3) dessin
    }

    game_shutdown(&game);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
