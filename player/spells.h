#pragma once
typedef enum { AB_FIREBALL, AB_DASH, AB_DOUBLEJUMP, AB_COUNT } AbilityId;

typedef struct {
    AbilityId id;
    const char* name;
    float mana_cost;
    float cooldown;    // secondes
    float cast_time;   // si canalisation
} AbilityDef;

typedef struct {
    AbilityId id;
    float cd_left;     // temps restant sur le cooldown
    int unlocked;      // 0/1 (Metroidvania: capacités acquises)
} AbilityState;