#include "render_explo.h"
#include "state_explo.h"
#include <time.h>
#include <../../include/SDL2/SDL.h>
#include <stdlib.h>

static const char TEST_MAP[] =
"#################################"
"#...............#..#............#"
"#..######.......#..#.##.###.....#"
"#..#....#..........#....###..#..#"
"#..#....#######....#....###..#..#"
"#...............................#"
"#################################";

// static int map_is_wall(const Map* m, int tx, int ty) {
//     if (tx < 0 || ty < 0 || tx >= m->w || ty >= m->h) return 1;
//     return m->data[ty * m->w + tx] == '#';
// }

static int is_valid_spawn(const Map* m, int tx, int ty, float px, float py) {
    if (map_is_wall(m, tx, ty)) return 0;
    int px_tile = (int)(px / 32);
    int py_tile = (int)(py / 32);
    int dx = abs(tx - px_tile);
    int dy = abs(ty - py_tile);
    return (dx > 2 || dy > 2); // éloigné d'au moins 2 cases du joeur
}

void explo_enter(Game* g, Explo** ps) {
    (void)g;
    srand((unsigned)time(NULL));
    if (!*ps) *ps = (Explo*)calloc(1, sizeof(Explo));

    (*ps)->map.w = 33;
    (*ps)->map.h = 7;
    (*ps)->map.data = TEST_MAP;

    (*ps)->player.x = 64;
    (*ps)->player.y = 64;

    // Spawn des ennemis
    (*ps)->enemy_count = 4;
    for (int i = 0; i < (*ps)->enemy_count; ++i) {
        int tx, ty;
        do {
            tx = rand() % (*ps)->map.w;
            ty = rand() % (*ps)->map.h;
        } while (!is_valid_spawn(&(*ps)->map, tx, ty,
                                 (*ps)->player.x, (*ps)->player.y));
        float ex = tx * 32 + 16;
        float ey = ty * 32 + 16;
        enemy_init(&(*ps)->ennemies[i], ex, ey);
    }
}

void explo_leave(Game* g, Explo** ps) {
    (void)g;
    free(*ps); *ps = NULL;
}

void explo_update(Game* g, Explo* s, float dt) {
    const Uint8* ks = SDL_GetKeyboardState(NULL);

    // Déplacements simples (32px la tuile)
    float speed = 150.f;
    float ax = 0, ay = 0;
    if (ks[SDL_SCANCODE_LEFT]  || ks[SDL_SCANCODE_A]) ax -= speed;
    if (ks[SDL_SCANCODE_RIGHT] || ks[SDL_SCANCODE_D]) ax += speed;
    if (ks[SDL_SCANCODE_UP]    || ks[SDL_SCANCODE_W]) ay -= speed;
    if (ks[SDL_SCANCODE_DOWN]  || ks[SDL_SCANCODE_S]) ay += speed;

    float nx = s->player.x + ax * dt;
    float ny = s->player.y + ay * dt;

    int tx = (int)(nx / 32.0f);
    int ty = (int)(s->player.y / 32.0f);
    if (!map_is_wall(&s->map, tx, ty)) s->player.x = nx;
    tx = (int)(s->player.x / 32.0f);
    ty = (int)(ny / 32.0f);
    if (!map_is_wall(&s->map, tx, ty)) s->player.y = ny;

    // Mise à jour des ennemis
    for (int i = 0; i < s->enemy_count; ++i)
        enemy_update(&s->ennemies[i], (Map*)&s->map, (Player*)&s->player);

    // collision/combat state change
    for (int i = 0; i < s->enemy_count; ++i) {
        Enemy* e = &s->ennemies[i];
        if (player_enemy_collide(&s->player, e)) {
            SDL_Log("Combat!");
            game_change_state(g, GS_COMBAT);
            return; // out of state_explo
        }
    }

    // Transitions debug
    if (ks[SDL_SCANCODE_X]) game_change_state(g, GS_COMBAT);
    if (ks[SDL_SCANCODE_R]) game_change_state(g, GS_HUB);
}

void explo_render(Game* g, Explo* s) {
    render_explo(g->ren, s);
    for (int i = 0; i < s->enemy_count; ++i)
        enemy_render(&s->ennemies[i], g->ren);
}