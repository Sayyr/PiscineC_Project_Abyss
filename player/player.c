#include "player.h"
#include "items.h"
#include "spells.h"
#include <string.h>
#include <math.h>

void player_init(Player* p) {
    memset(p, 0, sizeof *p);
    p->facing = +1;
    p->stats = (Stats){ .lvl=1, .str=5, .agi=5, .psy=5, .max_hp=100, .max_mp=30, .max_ox=100 };
    stats_compute_derived(&p->stats);
    // capacités de départ :
    p->abilities[AB_DASH].id = AB_DASH; p->abilities[AB_DASH].unlocked = 0; // à débloquer
    p->abilities[AB_DOUBLEJUMP].id = AB_DOUBLEJUMP; p->abilities[AB_DOUBLEJUMP].unlocked = 0;
    p->abilities[AB_FIREBALL].id = AB_FIREBALL; p->abilities[AB_FIREBALL].unlocked = 1; // ex
}

void player_tick_cooldowns(Player* p, float dt) {
    for (int i=0;i<AB_COUNT;i++)
        if (p->abilities[i].cd_left > 0) p->abilities[i].cd_left -= dt;
    if (p->i_frames > 0) p->i_frames -= dt;
}

// static const AbilityDef* ability_def(AbilityId id); // depuis la “DB” (fichier)
// static const ItemDef*    item_def(int item_id);     // idem

void player_apply_cmd(Player* p, const PlayerCmd* c, float dt) {
    // Mouvement horizontal
    float move = 0.0f;
    if (c->move_l) move -= 1.0f;
    if (c->move_r) move += 1.0f;
    if (move != 0) p->facing = (move < 0) ? -1 : +1;

    float base_speed = p->stats.move_speed; // dérivée calculée
    p->vx = move * base_speed;

    // Dash (si débloqué + cd ok)
    if (c->dash && p->abilities[AB_DASH].unlocked) {
        AbilityState* as = &p->abilities[AB_DASH];
        const AbilityDef* def = ability_def(AB_DASH);
        if (as->cd_left <= 0 /* && MP suffisant, etc. */) {
            p->vx = p->facing * 600.0f;
            as->cd_left = def->cooldown;
            // frames d’invulnérabilité ?
            p->i_frames = 0.15f;
        }
    }

    // Cast (capacité 1)
    if (c->cast1 && p->abilities[AB_FIREBALL].unlocked) {
        AbilityState* as = &p->abilities[AB_FIREBALL];
        const AbilityDef* def = ability_def(AB_FIREBALL);
        if (as->cd_left <= 0 /* && mp ok */) {
            // TODO: spawn projectile ici (entité)
            as->cd_left = def->cooldown;
            p->state = PST_CAST;
        }
    }

    // Utiliser un item d’inventaire
    if (c->use_item) {
        ItemStack used;
        if (inv_use(&p->inv, c->use_slot, &used)) {
            const ItemDef* d = item_def(used.item_id);
            // appliquer effets simples :
            p->stats.max_hp += d->hp_delta > 0 ? 0 : 0; // si potion perma, sinon applique à une ressource courante
            // si on gères HP courants, applique sur `hp_current` (à ajouter dans Stats ou à part)
        }
    }

    // Gravité + intégration
    // p->vy += 900.0f * dt;           // gravité simple
    // p->x  += p->vx * dt;
    // p->y  += p->vy * dt;

    // collisions sol/murs ici (tilemap) → ajuste p->x/p->y et flags (on_ground)
    // p->state = (fabsf(p->vx) > 1.f && p->on_ground)? PST_RUN : PST_IDLE;
}