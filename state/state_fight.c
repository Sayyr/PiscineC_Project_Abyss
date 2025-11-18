#include "state_fight.h"
#include <../../include/SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "render/render_fight.h"

void fight_enter(Game* g, Fight** ps) {
    (void)g;
    if (!*ps) *ps = (Fight*)calloc(1, sizeof(Fight));
    Fight* s = *ps;

    // TODO plus tard : prendre les vraies stats depuis g->explo->player, et l’Enemy qui a touché
    s->joueur_pv      = 100;
    s->joueur_pv_max  = 100;
    s->joueur_vitesse = 8;
    s->joueur_force   = 10;
    s->joueur_defense = 5;

    // Pour l’instant, on prend toujours le même mob (à ajouter le mob en param depuis explo)
    strcpy(s->mob_nom,         "3arbi Normal");
    s->mob_pv       = 50;
    s->mob_pv_max   = 50;
    s->mob_vitesse  = 6;
    s->mob_attaque  = 8;
    s->mob_defense  = 3;
    strcpy(s->mob_recompense,  "20 pieces d'or");

    // Init du round : qui commence ?
    s->gagnant       = 0;
    s->menu_actif    = 0;
    s->index_menu    = 0;
    s->index_attaque = 0;
    s->index_objet   = 0;

    if (s->joueur_vitesse >= s->mob_vitesse) {
        s->round = 0; // joueur
    } else {
        s->round = 1; // mob
    }

    snprintf(s->message, sizeof(s->message), "Combat contre %s!\n", s->mob_nom);
    s->message_timer = 60;
}
void fight_leave(Game* g, Fight** ps) {
    (void)g;
    free(*ps); *ps = NULL;
}

void fight_update(Game* g, Fight* s, float dt) {
    (void)dt;
    SDL_Event e;
    const Uint8* ks = SDL_GetKeyboardState(NULL);

    // décrémenter le timer de message
    if (s->message_timer > 0) {
        s->message_timer--;
    }
    // input
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            g->running = false;
            return;
        }
        // Debug: V = victoire (retour explo), L = défaite (retour hub)
        if (e.type == SDL_KEYDOWN) {
            if (ks[SDL_SCANCODE_V]) {
                game_change_state(g, GS_EXPLORATION);
                return;
            }
            if (ks[SDL_SCANCODE_L]) {
                game_change_state(g, GS_HUB);
                return;
            }
        }

        // si combat fini, une touche pour sortir (temporaire)
        if (s->gagnant == 1 && e.type == SDL_KEYDOWN) {
            game_change_state(g, GS_EXPLORATION); // ou GS_HUB etc.
            return;
        }
        // j'ai gardé ta logique que t'avais fait mais je l'ai adapté au projet
    }

    // 1) tour du joueur
    if (s->round == 0 && s->gagnant == 0 && e.type == SDL_KEYDOWN) {
            SDL_Keycode key = e.key.keysym.sym;

            // ----- MENU PRINCIPAL : ATTAQUE / OBJET / FUITE -----
            if (s->menu_actif == 0) {
                if (key == SDLK_UP) {
                    s->index_menu--;
                    if (s->index_menu < 0) s->index_menu = 2;
                } else if (key == SDLK_DOWN) {
                    s->index_menu++;
                    if (s->index_menu > 2) s->index_menu = 0;
                } else if (key == SDLK_RETURN || key == SDLK_SPACE) {
                    switch (s->index_menu) {
                    case 0: // ATTAQUE
                        s->menu_actif = 1;
                        s->index_attaque = 0;
                        break;
                    case 1: // OBJET
                        s->menu_actif = 2;
                        s->index_objet = 0;
                        break;
                    case 2: // FUITE
                        snprintf(s->message, sizeof(s->message),
                                 "Vous avez fui le combat!\n");
                        printf("Negunudayoooo smokeyyyy !\n");
                        s->message_timer = 60;
                        s->gagnant = 1;
                        break;
                    }
                }
            }
            // ----- MENU ATTAQUES -----
            else if (s->menu_actif == 1) {
                if (key == SDLK_UP) {
                    s->index_attaque--;
                    if (s->index_attaque < 0) s->index_attaque = 3;
                } else if (key == SDLK_DOWN) {
                    s->index_attaque++;
                    if (s->index_attaque > 3) s->index_attaque = 0;
                } else if (key == SDLK_ESCAPE) {
                    // retour au menu principal
                    s->menu_actif = 0;
                } else if (key == SDLK_RETURN || key == SDLK_SPACE) {
                    // Appliquer une attaque
                    int degats_base[] = {5, 7, 10, 15};
                    int degats = degats_base[s->index_attaque]
                                 + s->joueur_force
                                 - s->mob_defense;
                    if (degats < 1) degats = 1;
                    s->mob_pv -= degats;

                    snprintf(s->message, sizeof(s->message),
                             "Vous infligez %d degats!\n", degats);
                    s->message_timer = 60;

                    if (s->mob_pv <= 0) {
                        s->mob_pv = 0;
                        snprintf(s->message, sizeof(s->message),
                                 "Victoire! Vous obtenez: %s\n", s->mob_recompense);
                        printf("c'est pas vrai mais faites comme..\n");
                        s->message_timer = 120;
                        s->gagnant = 1;
                    } else {
                        // à l'ennemi de jouer
                        s->round = 1;
                    }
                    // on revient au menu principal après l'attaque
                    s->menu_actif = 0;
                }
            }
            // ----- MENU OBJETS -----
            else if (s->menu_actif == 2) {
                if (key == SDLK_UP) {
                    s->index_objet--;
                    if (s->index_objet < 0) s->index_objet = 2;
                } else if (key == SDLK_DOWN) {
                    s->index_objet++;
                    if (s->index_objet > 2) s->index_objet = 0;
                } else if (key == SDLK_ESCAPE) {
                    s->menu_actif = 0;
                } else if (key == SDLK_RETURN || key == SDLK_SPACE) {
                    // Appliquer un objet simple (placeholders)
                    if (s->index_objet == 0) {
                        s->joueur_pv += 30;
                        if (s->joueur_pv > s->joueur_pv_max)
                            s->joueur_pv = s->joueur_pv_max;
                        snprintf(s->message, sizeof(s->message),
                                 "Vous recuperez 30 PV!\n");
                    } else if (s->index_objet == 1) {
                        s->joueur_pv += 50;
                        if (s->joueur_pv > s->joueur_pv_max)
                            s->joueur_pv = s->joueur_pv_max;
                        snprintf(s->message, sizeof(s->message),
                                 "Vous recuperez 50 PV!\n");
                    } else if (s->index_objet == 2) {
                        s->joueur_force += 5;
                        snprintf(s->message, sizeof(s->message),
                                 "Votre force augmente de 5!\n");
                    }
                    s->message_timer = 60;
                    // ensuite, tour de l’ennemi
                    s->round = 1;
                    s->menu_actif = 0;
                }
            }
        } // fin "if round joueur"
    
    // 2) round de l'ennemi (quand message_timer == 0, gagnant pas 1, round == 1)
    if (s->round == 1 && s->gagnant != 1 && s->message_timer == 0) {
        // logique de l’attaque du mob (copier/coller et adapter du proto)
        int choix = rand() % 4;
        int degats_base[] = {6, 8, 10, 12};
        int degats = degats_base[choix] + s->mob_attaque - s->joueur_defense;
        if (degats < 1) degats = 1;
        s->joueur_pv -= degats;

        snprintf(s->message, sizeof(s->message),
                 "%s vous inflige %d degats!\n", s->mob_nom, degats);
        s->message_timer = 60;

        if (s->joueur_pv <= 0) {
            s->joueur_pv = 0;
            snprintf(s->message, sizeof(s->message), "Defaite...\n");
            s->message_timer = 120;
            s->gagnant = 1;
            game_change_state(g, GS_HUB); //en com pour debug
        } else {
            s->round = 0;
        }
    }
}

void fight_render(Game* g, Fight* s) {
    render_fight(g->ren, s);
}
