#include "render/render_explo.h"
#include "state_explo.h"
#include <SDL.h>
#include <stdlib.h>

static const char TEST_MAP[] =
"####################"
"#...............#..#"
"#..######.......#..#"
"#..#....#..........#"
"#..#....#######....#"
"#..................#"
"####################";

static int map_is_wall(const DemoMap* m, int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= m->w || ty >= m->h) return 1;
    return m->data[ty * m->w + tx] == '#';
}

void explo_enter(Game* g, Explo** ps) {
    (void)g;
    if (!*ps) *ps = (Explo*)calloc(1, sizeof(Explo));
    (*ps)->map.w = 20; (*ps)->map.h = 7; (*ps)->map.data = TEST_MAP;
    (*ps)->player.x = 64; (*ps)->player.y = 64;
}
void explo_leave(Game* g, Explo** ps) {
    (void)g;
    free(*ps); *ps = NULL;
}

void explo_update(Game* g, Explo* s, float dt) {
    (void)dt;
    const Uint8* ks = SDL_GetKeyboardState(NULL);

    // Déplacements simples (32px la tuile)
    float speed = 150.f; float ax = 0, ay = 0;
    if (ks[SDL_SCANCODE_LEFT]  || ks[SDL_SCANCODE_A]) ax -= speed;
    if (ks[SDL_SCANCODE_RIGHT] || ks[SDL_SCANCODE_D]) ax += speed;
    if (ks[SDL_SCANCODE_UP]    || ks[SDL_SCANCODE_W]) ay -= speed;
    if (ks[SDL_SCANCODE_DOWN]  || ks[SDL_SCANCODE_S]) ay += speed;

    float nx = s->player.x + ax * dt;
    float ny = s->player.y + ay * dt;

    int tx = (int)(nx / 32.0f), ty = (int)(s->player.y / 32.0f);
    if (!map_is_wall(&s->map, tx, ty)) s->player.x = nx;
    tx = (int)(s->player.x / 32.0f);   ty = (int)(ny / 32.0f);
    if (!map_is_wall(&s->map, tx, ty)) s->player.y = ny;

    // Transitions debug : X => combat ; R => hub
    if (ks[SDL_SCANCODE_X]) game_change_state(g, GS_COMBAT);
    if (ks[SDL_SCANCODE_R]) game_change_state(g, GS_HUB);
}
void explo_render(Game* g, Explo* s) {
    render_explo(g->ren, s);
}