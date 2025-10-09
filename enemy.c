#include "enemy.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static void enemy_update_default(Enemy* e, const Map* map, const Player* player) {
    float dx = player->x - e->x;
    float dy = player->y - e->y;
    float dist = sqrtf(dx*dx + dy*dy);
    float detection_radius = 128.0f;

    if (dist < detection_radius && has_line_of_sight(map, e->x, e->y, player->x, player->y)) {
        e->last_player_x = player->x;
        e->last_player_y = player->y;
        e->has_last_player_pos = 1;
        int sx = (int)(e->x / 32), sy = (int)(e->y / 32);
        int ex = (int)(player->x / 32), ey = (int)(player->y / 32);
        Node path[MAX_PATH];
        int path_len = astar(map, sx, sy, ex, ey, path);
        if (path_len > 0) {
            float tx = path[0].x * 32 + 16;
            float ty = path[0].y * 32 + 16;
            float pdx = tx - e->x, pdy = ty - e->y;
            float plen = sqrtf(pdx*pdx + pdy*pdy);
            float speed = 60.0f * (1.0f/60.0f);
            if (plen > 1.0f) {
                e->x += (pdx/plen) * speed;
                e->y += (pdy/plen) * speed;
            }
        }
    } else if (e->has_last_player_pos) {
        float dx = e->last_player_x - e->x;
        float dy = e->last_player_y - e->y;
        float dist = sqrtf(dx*dx + dy*dy);
        if (dist > 4.0f) {
            int sx = (int)(e->x / 32), sy = (int)(e->y / 32);
            int ex = (int)(e->last_player_x / 32), ey = (int)(e->last_player_y / 32);
            Node path[MAX_PATH];
            int path_len = astar(map, sx, sy, ex, ey, path);
            if (path_len > 0) {
                float tx = path[0].x * 32 + 16;
                float ty = path[0].y * 32 + 16;
                float pdx = tx - e->x, pdy = ty - e->y;
                float plen = sqrtf(pdx*pdx + pdy*pdy);
                float speed = 60.0f * (1.0f/60.0f);
                if (plen > 1.0f) {
                    e->x += (pdx/plen) * speed;
                    e->y += (pdy/plen) * speed;
                }
            }
        } else {
            e->has_last_player_pos = 0;
        }
    } else {
        enemy_wander(e, map);
    }

    // Collision joueur/ennemi (exemple)
    float pdx = player->x - e->x;
    float pdy = player->y - e->y;
    float pdist = sqrtf(pdx*pdx + pdy*pdy);
    if (pdist < 24.0f) {
        printf("attaque!\n");
    }
}

static void enemy_render_default(Enemy* e, SDL_Renderer* ren) {
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    // Simple cercle rouge
    for (int w = -12; w <= 12; ++w) {
        for (int h = -12; h <= 12; ++h) {
            if (w*w + h*h <= 12*12)
                SDL_RenderDrawPoint(ren, (int)(e->x + w), (int)(e->y + h));
        }
    }
}

static const EnemyVTable default_enemy_vtable = {
    .update = enemy_update_default,
    .render = enemy_render_default
};

void enemy_init(Enemy* e, float x, float y) {
    e->x = x;
    e->y = y;
    e->hp = 3;
    e->last_player_x = 0;
    e->last_player_y = 0;
    e->has_last_player_pos = 0;
    e->wander_target_x = -1;
    e->wander_target_y = -1;
    e->vtable = &default_enemy_vtable;
}

void enemy_update(Enemy* e, const Map* map, const Player* player) {
    if (e->vtable && e->vtable->update)
        e->vtable->update(e, map, player);
}

void enemy_render(Enemy* e, SDL_Renderer* ren) {
    if (e->vtable && e->vtable->render)
        e->vtable->render(e, ren);
}

void enemy_wander(Enemy* e, const Map* map) {
    if (e->wander_target_x == -1 && e->wander_target_y == -1) {
        int tx, ty;
        do {
            tx = rand() % map->w;
            ty = rand() % map->h;
        } while (map_is_wall(map, tx, ty));
        e->wander_target_x = tx;
        e->wander_target_y = ty;
    }
    int sx = (int)(e->x / 32), sy = (int)(e->y / 32);
    int ex = e->wander_target_x, ey = e->wander_target_y;
    Node path[MAX_PATH];
    int path_len = astar(map, sx, sy, ex, ey, path);
    if (path_len > 0) {
        float tx = path[0].x * 32 + 16;
        float ty = path[0].y * 32 + 16;
        float pdx = tx - e->x, pdy = ty - e->y;
        float plen = sqrtf(pdx*pdx + pdy*pdy);
        float speed = 60.0f * (1.0f/60.0f);
        if (plen > 1.0f) {
            e->x += (pdx/plen) * speed;
            e->y += (pdy/plen) * speed;
        }
        if (fabsf(e->x - (ex*32+16)) < 4.0f && fabsf(e->y - (ey*32+16)) < 4.0f) {
            e->wander_target_x = -1;
            e->wander_target_y = -1;
        }
    } else {
        e->wander_target_x = -1;
        e->wander_target_y = -1;
    }
}