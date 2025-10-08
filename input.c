#include "input.h"
#include <SDL2/SDL.h>

Input input_read(bool* running) {
    Input in = {0};
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) *running = false;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) *running = false;
    }
    const Uint8* ks = SDL_GetKeyboardState(NULL);
    in.left  = ks[SDL_SCANCODE_A] || ks[SDL_SCANCODE_LEFT];
    in.right = ks[SDL_SCANCODE_D] || ks[SDL_SCANCODE_RIGHT];
    in.up    = ks[SDL_SCANCODE_W] || ks[SDL_SCANCODE_UP];
    in.down  = ks[SDL_SCANCODE_S] || ks[SDL_SCANCODE_DOWN];
    return in;
}
