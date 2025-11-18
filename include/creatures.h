#ifndef CREATURES_H
#define CREATURES_H

#define MAX_CREATURES 4

typedef enum {
    CREATURE_KRAKEN,
    CREATURE_SHARK,
    CREATURE_JELLYFISH,
    CREATURE_SWORDFISH,
    CREATURE_CRAB
} CreatureType;

typedef struct {
    int id;
    CreatureType type;
    char name[30];
    int max_health;
    int current_health;
    int attack_min;
    int attack_max;
    int defense;
    int speed;
    char special_effect[20];
    int alive;
} MarineCreature;

void creatures_generate(MarineCreature* creatures, int* count, int depth);
void creature_take_damage(MarineCreature* creature, int damage);
int creature_get_attack_damage(MarineCreature* creature);
const char* get_creature_name(CreatureType type);

#endif