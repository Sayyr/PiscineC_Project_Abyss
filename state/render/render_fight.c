// render/render_fight.c
#include "render_fight.h"
#include "../state_fight.h"
#include <math.h>

// SDL_GetRendererOutputSize??
// j'ai hésite, mais pour les tests on va garder une taille fixe
#define W  800
#define H 600

static void draw_rect(SDL_Renderer* r, int x, int y, int w, int h,
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
    draw_rect(r, x, y, largeur, 20, 0, 0, 0, 255);

    float ratio = (pv_max > 0) ? (float)pv_actuel / (float)pv_max : 0.f;
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;
    int largeur_vie = (int)(largeur * ratio);

    if (ratio > 0.5f) {
        draw_rect(r, x, y, largeur_vie, 20, 0, 255, 0, 255);
    } else if (ratio > 0.2f) {
        draw_rect(r, x, y, largeur_vie, 20, 255, 165, 0, 255);
    } else {
        draw_rect(r, x, y, largeur_vie, 20, 255, 0, 0, 255);
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
    draw_rect(r, mob_x, mob_y, 160, 160, 139, 0, 0, 0);
    draw_border(r, mob_x, mob_y, 160, 160, 0, 0, 0);

    // yeux blancs
    draw_rect(r, mob_x+40,  mob_y+40, 20,20,20, 255,255,255);
    draw_rect(r, mob_x+100, mob_y+40, 20,20,20, 255,255,255);

    // pupilles
    draw_rect(r, mob_x+45,  mob_y+45, 10,10,10, 0,0,0);
    draw_rect(r, mob_x+105, mob_y+45, 10,10,10, 0,0,0);

    // bouche
    draw_rect(r, mob_x+60, mob_y+100, 40,10,0, 0,0,0);

    // === BARRE DE VIE ENNEMI ===
    draw_rect(r, W-220, 20, 200, 80,0, 0,0,0);
    draw_hp_bar(r, W-210, 60, s->mob_pv, s->mob_pv_max, 180);

    // === PANEL BAS ===
    int ui_y = H - 200;
    draw_rect  (r, 0, ui_y, W, 200,200, 240,240,240);
    draw_border(r, 0, ui_y, W, 200, 0,0,0);

    // === MENU PRINCIPAL (ATTAQUE / OBJET / FUITE) ===

    int x = 50;
    int y = ui_y + 30;
    int bw = 130, bh = 30;

    // surbrillance jaune
    if (s->menu_actif == 0 && s->index_menu == 0)
        draw_rect(r, x-10, y-5, bw+20, bh+10,0, 255,200,0);

    // bouton ATTAQUE (rouge clair)
    draw_rect  (r, x, y, bw, bh, 200,50,50,50);
    draw_border(r, x, y, bw, bh, 0,0,0);

    y += 50;

    if (s->menu_actif == 0 && s->index_menu == 1)
        draw_rect(r, x-10, y-5, bw+20, bh+10,0, 255,200,0);

    // bouton OBJET (bleu)
    draw_rect  (r, x, y, bw, bh,0, 50,100,200);
    draw_border(r, x, y, bw, bh, 0,0,0);

    y += 50;

    if (s->menu_actif == 0 && s->index_menu == 2)
        draw_rect(r, x-10, y-5, bw+20, bh+10,0, 255,200,0);

    // bouton FUITE (gris)
    draw_rect  (r, x, y, bw, bh,0, 100,100,100);
    draw_border(r, x, y, bw, bh, 0,0,0);

    // === PANEL JOUEUR ===
    draw_rect(r, W-250, ui_y+30, 230, 60,0, 0,0,0);
    draw_hp_bar(r, W-240, ui_y+50, s->joueur_pv, s->joueur_pv_max, 210);

    // === MESSAGE TEMPOREL (gros rectangle noir/jaune) ===
    if (s->message_timer > 0) {
        int mw = 500, mh = 80;
        int mx = W/2 - mw/2;
        int my = H/2 - mh/2;
        draw_rect  (r, mx, my, mw, mh,0, 0,0,0);
        draw_border(r, mx, my, mw, mh, 255,255,0);
    }
}
// void render_fight(SDL_Renderer* r, const struct Fight* s){
//     SDL_SetRenderDrawColor(r, 135, 206, 235, 255);
//     SDL_RenderClear(r);

//     // ==== ZONE ENNEMI ====
//     int mob_box_w = 160;
//     int mob_box_h = 160;
//     int mob_box_x = W / 2 - mob_box_w / 2;
//     int mob_box_y = 80;

//     // Corps du mob
//     draw_rect(r, mob_box_x, mob_box_y, mob_box_w, mob_box_h, 139, 0, 0, 255);
//     // Bord
//     draw_border(r, mob_box_x, mob_box_y, mob_box_w, mob_box_h, 0, 0, 0);

//     // placeholder
//     draw_rect(r, mob_box_x + 40,  mob_box_y + 40, 20, 20, 255, 255, 255, 255);
//     draw_rect(r, mob_box_x + 100, mob_box_y + 40, 20, 20, 255, 255, 255, 255);
//     draw_rect(r, mob_box_x + 45,  mob_box_y + 45, 10, 10, 0,   0,   0,   255);
//     draw_rect(r, mob_box_x + 105, mob_box_y + 45, 10, 10, 0,   0,   0,   255);
//     draw_rect(r, mob_box_x + 60,  mob_box_y + 100, 40, 10, 0,  0,   0,   255);

//     // ==== BARRE DE VIE ENNEMI ====
//     int mob_panel_x = W - 220;
//     int mob_panel_y = 20;
//     int mob_panel_w = 200;
//     int mob_panel_h = 80;

//     draw_rect(r, mob_panel_x, mob_panel_y, mob_panel_w, mob_panel_h, 0, 0, 0, 200);
//     draw_hp_bar(r, mob_panel_x + 10, mob_panel_y + 40,
//                 s->mob_pv, s->mob_pv_max, mob_panel_w - 20);

//     // (afficher s->mob_nom au-dessus)

//     // ==== INTERFACE DU BAS ====
//     int interface_h = 200;
//     int interface_y = H - interface_h;

//     draw_rect(r, 0, interface_y, W, interface_h, 240, 240, 240, 255);
//     draw_border(r, 0, interface_y, W, interface_h, 0, 0, 0);

//     // Menu principal : 3 boutons verticaux (ATTAQUE / OBJET / FUITE)
//     int menu_x = 50;
//     int menu_y = interface_y + 30;
//     int espacement = 50;
//     int bw = 130, bh = 30;

//     // Bouton ATTAQUE
//     if (s->menu_actif == 0 && s->index_menu == 0) {
//         draw_rect(r, menu_x - 10, menu_y - 5, bw + 20, bh + 10, 255, 200, 0, 255);
//     }
//     draw_rect(r,   menu_x, menu_y, bw, bh, 200, 50, 50, 255);
//     draw_border(r, menu_x, menu_y, bw, bh, 0, 0, 0);

//     // Bouton OBJET
//     menu_y += espacement;
//     if (s->menu_actif == 0 && s->index_menu == 1) {
//         draw_rect(r, menu_x - 10, menu_y - 5, bw + 20, bh + 10, 255, 200, 0, 255);
//     }
//     draw_rect(r,   menu_x, menu_y, bw, bh, 50, 100, 200, 255);
//     draw_border(r, menu_x, menu_y, bw, bh, 0, 0, 0);

//     // la stratégie ancetrale des joeystars (la FUITE)
//     menu_y += espacement;
//     if (s->menu_actif == 0 && s->index_menu == 2) {
//         draw_rect(r, menu_x - 10, menu_y - 5, bw + 20, bh + 10, 255, 200, 0, 255);
//     }
//     draw_rect(r,   menu_x, menu_y, bw, bh, 100, 100, 100, 255);
//     draw_border(r, menu_x, menu_y, bw, bh, 0, 0, 0);

//     // ==== BARRE DE VIE JOUEUR ====
//     int player_panel_x = W - 250;
//     int player_panel_y = interface_y + 30;
//     int player_panel_w = 230;
//     int player_panel_h = 60;

//     draw_rect(r, player_panel_x, player_panel_y, player_panel_w, player_panel_h,
//               0, 0, 0, 200);
//     draw_hp_bar(r, player_panel_x + 10, player_panel_y + 20,
//                 s->joueur_pv, s->joueur_pv_max, player_panel_w - 20);

//     // ==== MESSAGE FLOTTANT ====
//     if (s->message_timer > 0) {
//         int mw = 500, mh = 80;
//         int mx = W/2 - mw/2;
//         int my = H/2 - mh/2;
//         draw_rect(r,   mx, my, mw, mh, 0, 0, 0, 230);
//         draw_border(r, mx, my, mw, mh, 255, 255, 0);
//         // ajouter SDL_ttf pour les messages
//     }
// }
