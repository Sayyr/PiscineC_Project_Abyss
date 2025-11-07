#pragma once
typedef enum { AB_NONE = 0, AB_FIREBALL, AB_DASH, AB_DOUBLEJUMP, AB_STRIKE, AB_COUNT } AbilityId;

typedef struct {
    AbilityId id;
    const char* name;
    float mana_cost;
    float power; //puissance
    float cooldown;    // secondes
    float cast_time;   // si canalisation, si 0 c'est que c'est instant (bah oe logik)
} AbilityDef;

typedef struct {
    AbilityId id;
    float cd_left;     // temps restant sur le cooldown
    int unlocked;      // 0/1
} AbilityState;

const AbilityDef* ability_def(AbilityId id);