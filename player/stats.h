#pragma once
typedef struct {
    int lvl;
    int xp;
    float str, agi, psy;   // stats de base
    float max_hp, max_mp, max_ox;
    float atk, def, crit, move_speed; // dérivées
} Stats;

void stats_compute_derived(Stats* s);