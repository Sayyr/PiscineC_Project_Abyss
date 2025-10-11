#pragma once
#include "stats.h"

typedef enum { ST_POISON, ST_SLOW, ST_INVINCIBLE, ST_COUNT } StatusType;
typedef struct {
    StatusType type;
    float time_left;     // secondes
    float magnitude;     // intensité (x%)
} StatusEffect;

#define MAX_STATUS 8
typedef struct {
    StatusEffect effects[MAX_STATUS];
    int count;
} StatusList;

void status_tick(StatusList* sl, float dt, Stats* s); // applique/retire effets