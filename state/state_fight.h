#pragma once
#include "game.h"

typedef struct Fight {
    int joueur_pv;
    int joueur_pv_max;
    int joueur_vitesse;
    int joueur_force;
    int joueur_defense;

    char mob_nom[50];
    int mob_pv;
    int mob_pv_max;
    int mob_vitesse;
    int mob_attaque;
    int mob_defense;
    char mob_recompense[50];

    // État du combat
    int gagnant;        // 0 = en cours, 1 = terminé
    int round;           // 0 = joueur, 1 = mob
    int menu_actif;     // 0 = menu principal, 1 = menu attaques, 2 = objets
    int index_menu;     
    int index_attaque;
    int index_objet;
    int message_timer;  // frames restantes pour afficher le message
    char message[200];
} Fight;

void fight_enter (Game* g, Fight** ps);
void fight_update(Game* g, Fight*  s, float dt);
void fight_render(Game* g, Fight*  s);
void fight_leave (Game* g, Fight** ps);
