#include "game.h"
#include "state/state_hub.h"
#include "state/state_explo.h"
#include "state/state_fight.h"

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    Game g = {0};
    if (!game_init(&g, "Abyss", 960, 540)) return 1;

    // état initial
    g.state = g.next_state = GS_HUB;
    hub_enter(&g, &g.hub);

    Uint64 last = SDL_GetPerformanceCounter();
    const double freq = (double)SDL_GetPerformanceFrequency();

    while (g.running) {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)((now - last) / freq);
        if (dt > 0.25f) dt = 0.25f;
        last = now;

        // events globaux
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) g.running = false;
        }

        // UPDATE + TRANSITIONS (centralisé dans game.c)
        game_step(&g, dt);

        // RENDER
        switch (g.state) {
            case GS_HUB:
                hub_render(&g, g.hub);
                break;
            case GS_EXPLORATION:
                explo_render(&g, g.explo);
                break;
            case GS_COMBAT:
                fight_render(&g, g.fight);
                break;
        }
        SDL_RenderPresent(g.ren);
    }

    // leave propre de l'état courant (c'est la propreté comme ça)
    switch (g.state) {
        case GS_HUB:         hub_leave(&g, &g.hub);       break;
        case GS_EXPLORATION: explo_leave(&g, &g.explo);   break;
        case GS_COMBAT:      fight_leave(&g, &g.fight); break;
    }

    game_shutdown(&g);
    return 0;
}
