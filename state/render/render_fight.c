// render/render_fight.c
#include "render_fight.h"
#include "../state_fight.h"
#include <math.h>

// SDL_GetRendererOutputSize??
// j'ai hésite, mais pour les tests on va garder une taille fixe
#define W  800
#define H 600

static void draw_rect_a(SDL_Renderer* r, int x, int y, int w, int h,
                      int R, int G, int B, int A)
{
    SDL_Rect rect = { x, y, w, h };
    SDL_SetRenderDrawColor(r, R, G, B, A);
    SDL_RenderFillRect(r, &rect);
}

static void draw_border(SDL_Renderer* r, int x, int y, int w, int h,
                        int R, int G, int B)
{
    SDL_Rect rect = { x, y, w, h };
    SDL_SetRenderDrawColor(r, R, G, B, 255);
    SDL_RenderDrawRect(r, &rect);
}

static void draw_hp_bar(SDL_Renderer* r, int x, int y,
                        int pv_actuel, int pv_max, int largeur)
{
    // fond
    draw_rect(r, x, y, largeur, 20, 0, 0, 0);

    float ratio = (pv_max > 0) ? (float)pv_actuel / (float)pv_max : 0.f;
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;
    int largeur_vie = (int)(largeur * ratio);

    if (ratio > 0.5f) {
        draw_rect(r, x, y, largeur_vie, 20, 0, 255, 0);
    } else if (ratio > 0.2f) {
        draw_rect(r, x, y, largeur_vie, 20, 255, 165, 0);
    } else {
        draw_rect(r, x, y, largeur_vie, 20, 255, 0, 0);
    }

    draw_border(r, x, y, largeur, 20, 255, 255, 255);
}

void render_fight(SDL_Renderer* r, const struct Fight* s)
{
    // === FOND bleu clair ===
    SDL_SetRenderDrawColor(r, 135, 206, 235, 255);
    SDL_RenderClear(r);

    // === ENNEMI (gros carré rouge avec yeux) ===
    int mob_x = W/2 - 80;
    int mob_y = 80;

    // corps
    draw_rect(r, mob_x, mob_y, 160, 160, 139, 0, 0);
    draw_border(r, mob_x, mob_y, 160, 160, 0, 0, 0);

    // yeux blancs
    draw_rect(r, mob_x+40,  mob_y+40, 20,20, 255,255,255);
    draw_rect(r, mob_x+100, mob_y+40, 20,20, 255,255,255);

    // pupilles
    draw_rect(r, mob_x+45,  mob_y+45, 10,10, 0,0,0);
    draw_rect(r, mob_x+105, mob_y+45, 10,10, 0,0,0);

    // bouche
    draw_rect(r, mob_x+60, mob_y+100, 40,10, 0,0,0);

    // === BARRE DE VIE ENNEMI ===
    draw_rect(r, W-220, 20, 200, 80, 0,0,0);
    draw_hp_bar(r, W-210, 60, s->mob_pv, s->mob_pv_max, 180);

    // === PANEL BAS ===
    int ui_y = H - 200;
    draw_rect  (r, 0, ui_y, W, 200, 240,240,240);
    draw_border(r, 0, ui_y, W, 200, 0,0,0);

    // === MENU PRINCIPAL (ATTAQUE / OBJET / FUITE) ===

    int x = 50;
    int y = ui_y + 30;
    int bw = 130, bh = 30;

    // surbrillance jaune
    if (s->menu_actif == 0 && s->index_menu == 0)
        draw_rect(r, x-10, y-5, bw+20, bh+10, 255,200,0);

    // bouton ATTAQUE (rouge clair)
    draw_rect  (r, x, y, bw, bh, 200,50,50);
    draw_border(r, x, y, bw, bh, 0,0,0);

    y += 50;

    if (s->menu_actif == 0 && s->index_menu == 1)
        draw_rect(r, x-10, y-5, bw+20, bh+10, 255,200,0);

    // bouton OBJET (bleu)
    draw_rect  (r, x, y, bw, bh, 50,100,200);
    draw_border(r, x, y, bw, bh, 0,0,0);

    y += 50;

    if (s->menu_actif == 0 && s->index_menu == 2)
        draw_rect(r, x-10, y-5, bw+20, bh+10, 255,200,0);

    // bouton FUITE (gris)
    draw_rect  (r, x, y, bw, bh, 100,100,100);
    draw_border(r, x, y, bw, bh, 0,0,0);

    // === PANEL JOUEUR ===
    draw_rect(r, W-250, ui_y+30, 230, 60, 0,0,0);
    draw_hp_bar(r, W-240, ui_y+50, s->joueur_pv, s->joueur_pv_max, 210);

    // === MESSAGE TEMPOREL (gros rectangle noir/jaune) ===
    if (s->message_timer > 0) {
        int mw = 500, mh = 80;
        int mx = W/2 - mw/2;
        int my = H/2 - mh/2;
        draw_rect  (r, mx, my, mw, mh, 0,0,0);
        draw_border(r, mx, my, mw, mh, 255,255,0);
    }
}