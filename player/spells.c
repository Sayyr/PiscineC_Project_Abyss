#include "spells.h"

// exemple; à compléter plus tard grâce à un fichier JSON à lire
static const AbilityDef ABIL_TABLE[] = {
    { .id = ABIL_STRIKE, .name = "Strike", .mana_cost = 1, .power = 5 },
    { .id = AB_DASH, .name = "Dash", .mana_cost = 2, .power = 5 },  
    { .id = AB_DOUBLEJUMP, .name = "Double Saut", .mana_cost = 2, .power = 5 },  
    { .id = AB_COUNT, .name = "Count", .mana_cost = 1, .power = 5 },
    { .id = AB_FIREBALL, .name = "Boule de Feu", .mana_cost = 3, .power = 5 } 
};
static const int ABIL_COUNT = sizeof(ABIL_TABLE)/sizeof(ABIL_TABLE[0]);

const AbilityDef* ability_def(AbilityId id) {
    for (int i = 0; i < ABIL_COUNT; ++i) {
        if (ABIL_TABLE[i].id == id) return &ABIL_TABLE[i];
    }
    return;
}
