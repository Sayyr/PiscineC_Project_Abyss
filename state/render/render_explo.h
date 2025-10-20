#pragma once
#include <SDL.h>
#include "render_common.h"
struct Explo;  // forward
void render_explo(SDL_Renderer* r, const struct Explo* s);
