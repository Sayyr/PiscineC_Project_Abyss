#pragma once
#include <../../include/SDL2/SDL.h>
#include "render_common.h"
struct Fight;
void render_fight(SDL_Renderer* r, const struct Fight* s);