#pragma once
#include <stdbool.h>
#include "stats.h"
#include "inventory.h"
#include "spells.h"
#include "status.h"

typedef enum { PST_IDLE, PST_RUN, PST_JUMP, PST_ATTACK, PST_CAST, PST_HURT } PlayerState;

typedef struct {
    // spatial
    float x, y, vx, vy;
    int facing; // -1 gauche / +1 droite
    // jeu
    Stats stats;
    Inventory inv;
    AbilityState abilities[AB_COUNT];
    StatusList status;
    // contrôle
    PlayerState state;
    bool on_ground;
    // timers
    float i_frames; // invincibilité après coup
} Player;

typedef struct {
    // ce que le contrôleur “demande” cette frame
    bool move_l, move_r, jump, attack, cast1, cast2, dash, use_item;
    int  use_slot; // slot inventaire à utiliser (si use_item)
} PlayerCmd;

void player_init(Player* p);
void player_apply_cmd(Player* p, const PlayerCmd* cmd, float dt);
void player_tick_cooldowns(Player* p, float dt);
void player_learn_ability(Player* p, AbilityId id);
void player_use_oxygen(Player* p, int amount);
