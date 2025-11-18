#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/creatures.h"

const char* get_creature_name(CreatureType type) {
    switch (type) {
        case CREATURE_KRAKEN: return "Kraken";
        case CREATURE_SHARK: return "Requin-Tigre";
        case CREATURE_JELLYFISH: return "Meduse Bleue";
        case CREATURE_SWORDFISH: return "Poisson-Epee";
        case CREATURE_CRAB: return "Crabe Geant";
        default: return "Creature Inconnue";
    }
}

void creatures_generate(MarineCreature* creatures, int* count, int depth) {
    srand(time(NULL));
    *count = (rand() % 3) + 1; // 1 à 3 créatures
    
    for (int i = 0; i < *count; i++) {
        CreatureType type = rand() % 5;
        creatures[i].id = i;
        creatures[i].type = type;
        creatures[i].alive = 1;
        
        // Définir les stats selon le type
        switch (type) {
            case CREATURE_KRAKEN:
                creatures[i].max_health = 120 + (depth / 10);
                creatures[i].attack_min = 25;
                creatures[i].attack_max = 40;
                creatures[i].defense = 10;
                creatures[i].speed = 3;
                strcpy(creatures[i].special_effect, "double_attaque");
                break;
            case CREATURE_SHARK:
                creatures[i].max_health = 80 + (depth / 15);
                creatures[i].attack_min = 15;
                creatures[i].attack_max = 25;
                creatures[i].defense = 5;
                creatures[i].speed = 8;
                strcpy(creatures[i].special_effect, "frenesie");
                break;
            case CREATURE_JELLYFISH:
                creatures[i].max_health = 30 + (depth / 20);
                creatures[i].attack_min = 8;
                creatures[i].attack_max = 15;
                creatures[i].defense = 2;
                creatures[i].speed = 5;
                strcpy(creatures[i].special_effect, "paralysie");
                break;
            case CREATURE_SWORDFISH:
                creatures[i].max_health = 70 + (depth / 15);
                creatures[i].attack_min = 18;
                creatures[i].attack_max = 28;
                creatures[i].defense = 7;
                creatures[i].speed = 6;
                strcpy(creatures[i].special_effect, "perforation");
                break;
            case CREATURE_CRAB:
                creatures[i].max_health = 100 + (depth / 10);
                creatures[i].attack_min = 12;
                creatures[i].attack_max = 20;
                creatures[i].defense = 15;
                creatures[i].speed = 2;
                strcpy(creatures[i].special_effect, "carapace");
                break;
        }
        
        creatures[i].current_health = creatures[i].max_health;
        snprintf(creatures[i].name, 30, "%s", get_creature_name(type));
    }
}

void creature_take_damage(MarineCreature* creature, int damage) {
    // Appliquer réduction dégâts pour crabe
    if (strcmp(creature->special_effect, "carapace") == 0) {
        damage = (int)(damage * 0.8); // -20% dégâts
    }
    
    creature->current_health -= damage;
    if (creature->current_health <= 0) {
        creature->current_health = 0;
        creature->alive = 0;
    }
}

int creature_get_attack_damage(MarineCreature* creature) {
    int damage = (rand() % (creature->attack_max - creature->attack_min + 1)) + creature->attack_min;
    
    // Appliquer les effets spéciaux
    if (strcmp(creature->special_effect, "frenesie") == 0 && 
        creature->current_health < creature->max_health / 2) {
        damage = (int)(damage * 1.3); // +30% dégâts
    }
    
    if (strcmp(creature->special_effect, "perforation") == 0) {
        damage += 2; // Ignore 2 points de défense
    }
    
    return damage;
}