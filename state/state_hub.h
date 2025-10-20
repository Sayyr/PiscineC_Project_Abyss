#pragma once
#include "game.h"
#include <stdbool.h>

typedef enum {
    HUB_UI_NONE = 0,
    HUB_UI_MERCHANT,
    HUB_UI_RECRUITER,
    HUB_UI_DEPART
} HubUi;

typedef struct Hub {
    // Carte statique du HUB
    int w, h;
    const char* data;  // '#' mur, '.' sol, 'M' marchand, 'R' recrutement, 'D' départ
    float x, y;        // Joueur (en pixels)
    HubUi ui;          // UI courante (menu ouvert ?)
} Hub;

void hub_enter (Game* g, Hub** ps);
void hub_update(Game* g, Hub*  s, float dt);
void hub_render(Game* g, Hub*  s);
void hub_leave (Game* g, Hub** ps);
