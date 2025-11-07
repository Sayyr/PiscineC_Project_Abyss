#include "spells.h"
#include <stddef.h>
#define ARRLEN(a) (sizeof(a)/sizeof((a)[0]))

// exemple; à compléter plus tard grâce à un fichier JSON à lire
static const AbilityDef ABIL_TABLE[] = {
    [AB_NONE] = {
        .id=AB_NONE, .name="None",
        .mana_cost=0.f, .power=0.f, .cooldown=0.f, .cast_time=0.f
    },
    [AB_STRIKE] = {
        .id=AB_STRIKE, .name="Strike",
        .mana_cost=1.f, .power=5.f, .cooldown=0.5f, .cast_time=0.f
    },
    [AB_DASH] = {
        .id=AB_DASH, .name="Dash",
        .mana_cost=2.f, .power=0.f, .cooldown=2.f, .cast_time=0.f
    },
    [AB_DOUBLEJUMP] = {
        .id=AB_DOUBLEJUMP, .name="Double Saut",
        .mana_cost=2.f, .power=0.f, .cooldown=0.f, .cast_time=0.f
    },
    [AB_FIREBALL] = {
        .id=AB_FIREBALL, .name="Boule de Feu",
        .mana_cost=3.f, .power=25.f, .cooldown=3.5f, .cast_time=0.2f
    },
};
_Static_assert(ARRLEN(ABIL_TABLE) == AB_COUNT,
               "ABIL_TABLE size must match AB_COUNT");

const AbilityDef* ability_def(AbilityId id) {
    for (int i = 0; i < AB_COUNT; ++i) {
        if (ABIL_TABLE[i].id == id) return &ABIL_TABLE[i];
    }
    return;
}

size_t ability_count(void) {
    return ARRLEN(ABIL_TABLE);
}