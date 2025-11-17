#include "state_hub.h"
#include "map.h"
#include <../../include/SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>

static const int TILE = 32;

// Carte HUB (12x8) : M = marchand, R = recruteur, D = départ (descendre), J = PNJ
static const char HUB_MAP[] =
"############"
"#..M.......#"
"#....#.#...#"
"#R...#D#...#"
"#....###...#"
"#..........#"
"#..........#"
"############";

static int is_wall(const Hub* s, int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= s->w || ty >= s->h) return 1;
    return s->data[ty * s->w + tx] == '#';
}

static char tile_at(const Hub* s, int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= s->w || ty >= s->h) return '#';
    return s->data[ty * s->w + tx];
}

// Renvoie code d'interaction
static HubUi detect_panel(const Hub* s, float px, float py) {
    int tx = (int)(px / TILE);
    int ty = (int)(py / TILE);
    // On regarde la tuile "devant" le joueur au sens large (4 directions)
    const int around[4][2] = { {+1,0},{-1,0},{0,+1},{0,-1} };
    for (int i=0;i<4;i++){
        int nx = tx + around[i][0], ny = ty + around[i][1];
        char t = tile_at(s, nx, ny);
        if (t == 'M') return HUB_UI_MERCHANT;
        if (t == 'R') return HUB_UI_RECRUITER;
        if (t == 'D') return HUB_UI_DEPART;
    }
    return HUB_UI_NONE;
}

void hub_enter(Game* g, Hub** ps) {
    (void)g;
    if (!*ps) *ps = (Hub*)calloc(1, sizeof(Hub));
    (*ps)->w = 12; (*ps)->h = 8;
    (*ps)->data = HUB_MAP;
    (*ps)->x = 2 * TILE + TILE/2;   // spawn près du marchand
    (*ps)->y = 1 * TILE + TILE/2;
    (*ps)->ui = HUB_UI_NONE;
}

void hub_leave(Game* g, Hub** ps) {
    (void)g;
    free(*ps); *ps = NULL;
}

void hub_update(Game* g, Hub* s, float dt) {
    const Uint8* ks = SDL_GetKeyboardState(NULL);

    // Quitter le jeu basiquement
    if (ks[SDL_SCANCODE_ESCAPE] && s->ui == HUB_UI_NONE) {
        g->running = false;
        return;
    }
    if (s->ui != HUB_UI_NONE) {
        // Choices "placeholder" : chiffre 1..9 pour sélectionner ; ESC pour fermer
        if (ks[SDL_SCANCODE_1]) {
            SDL_Log("[HUB] Choix 1 dans menu %d", s->ui);
        } else if (ks[SDL_SCANCODE_2]) {
            SDL_Log("[HUB] Choix 2 dans menu %d", s->ui);
        } else if (ks[SDL_SCANCODE_3]) {
            SDL_Log("[HUB] Choix 3 dans menu %d", s->ui);
        }
        // Départ : dès qu'on ouvre et qu'on appuie '1' par ex., on peut transiter
        if (s->ui == HUB_UI_DEPART && ks[SDL_SCANCODE_1]) {
            game_change_state(g, GS_EXPLORATION);
            return;
        }
        if (ks[SDL_SCANCODE_ESCAPE]) {
            s->ui = HUB_UI_NONE; // fermer le panneau
        }
        return;
    }

    // --- Déplacement libre dans la carte (collisions murs) ---
    float speed = 160.f;
    float ax = 0.f, ay = 0.f;
    if (ks[SDL_SCANCODE_LEFT]  || ks[SDL_SCANCODE_A]) ax -= speed;
    if (ks[SDL_SCANCODE_RIGHT] || ks[SDL_SCANCODE_D]) ax += speed;
    if (ks[SDL_SCANCODE_UP]    || ks[SDL_SCANCODE_W]) ay -= speed;
    if (ks[SDL_SCANCODE_DOWN]  || ks[SDL_SCANCODE_S]) ay += speed;

    float nx = s->x + ax * dt;
    float ny = s->y + ay * dt;

    // Collisions simples en X puis Y (tile 32)
    int tx = (int)(nx / TILE), ty = (int)(s->y / TILE);
    if (!is_wall(s, tx, ty)) s->x = nx;
    tx = (int)(s->x / TILE); ty = (int)(ny / TILE);
    if (!is_wall(s, tx, ty)) s->y = ny;

    // --- Ouverture d'un panneau si 'E' près d'un hotspot ---
    if (ks[SDL_SCANCODE_E]) {
        HubUi near = detect_panel(s, s->x, s->y);
        if (near != HUB_UI_NONE) {
            s->ui = near; // ouvre le menu correspondant
            SDL_Log("[HUB] Ouverture menu %d (1/2/3 pour choisir, ESC pour fermer)", s->ui);
        }
    }
}

static void draw_rect(SDL_Renderer* r, int x, int y, int w, int h, int R, int G, int B) {
    SDL_Rect rr = {x,y,w,h};
    SDL_SetRenderDrawColor(r, R,G,B,255);
    SDL_RenderFillRect(r, &rr);
}

void hub_render(Game* g, Hub* s) {
    // Fond
    SDL_SetRenderDrawColor(g->ren, 12, 14, 18, 255);
    SDL_RenderClear(g->ren);

    // Dessin de la carte
    for (int y = 0; y < s->h; ++y) {
        for (int x = 0; x < s->w; ++x) {
            char c = s->data[y * s->w + x];
            SDL_Rect t = { x*TILE, y*TILE, TILE, TILE };
            switch (c) {
                case '#': SDL_SetRenderDrawColor(g->ren, 40, 44, 52, 255); break; // mur
                case '.': SDL_SetRenderDrawColor(g->ren, 24, 26, 32, 255); break; // sol
                case 'M': SDL_SetRenderDrawColor(g->ren, 24, 26, 32, 255); break;
                case 'R': SDL_SetRenderDrawColor(g->ren, 24, 26, 32, 255); break;
                case 'D': SDL_SetRenderDrawColor(g->ren, 24, 26, 32, 255); break;
            }
            SDL_RenderFillRect(g->ren, &t);

            // Hotspots visibles (panneaux) : petits totems colorés
            if (c == 'M') draw_rect(g->ren, t.x+8, t.y+8, 16, 16, 70, 170, 255);   // marchand (bleu)
            if (c == 'R') draw_rect(g->ren, t.x+8, t.y+8, 16, 16, 130, 200, 90);  // recruteur (vert)
            if (c == 'D') draw_rect(g->ren, t.x+8, t.y+8, 16, 16, 220, 180, 60);  // départ (jaune)
        }
    }

    // Joueur (disque/carré jaune)
    SDL_Rect p = { (int)s->x - 10, (int)s->y - 10, 20, 20 };
    SDL_SetRenderDrawColor(g->ren, 240, 220, 80, 255);
    SDL_RenderFillRect(g->ren, &p);

    // UI: si un menu est ouvert, afficher un panneau opaque centré
    if (s->ui != HUB_UI_NONE) {
        int W = 360, H = 200;
        int X = (s->w*TILE - W)/2;
        int Y = (s->h*TILE - H)/2;
        // Fond du panneau
        draw_rect(g->ren, X, Y, W, H, 18, 20, 26);
        // Bord
        SDL_Rect bd = {X, Y, W, H};
        SDL_SetRenderDrawColor(g->ren, 90, 100, 120, 255);
        SDL_RenderDrawRect(g->ren, &bd);

        // Boutons placeholders (juste des rectangles cliquables au clavier)
        int bw = W - 40, bh = 36, bx = X + 20, by = Y + 20;
        // Légende par couleur (on n’a pas de texte sans SDL_ttf)
        draw_rect(g->ren, bx, by, bw, bh, 50, 120, 200);     // Option 1
        draw_rect(g->ren, bx, by+bh+12, bw, bh, 70, 150, 90);// Option 2
        draw_rect(g->ren, bx, by+2*(bh+12), bw, bh, 150, 80, 60); // Option 3
        // Indices inputs : utilise 1/2/3 pour choisir, ÉCHAP pour fermer (voir SDL_Log)
    }
}
