#pragma once
#include "game.h"

typedef struct Fight {
    int round;
} Fight;

void fight_enter (Game* g, Fight** ps);
void fight_update(Game* g, Fight*  s, float dt);
void fight_render(Game* g, Fight*  s);
void fight_leave (Game* g, Fight** ps);
