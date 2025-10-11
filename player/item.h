#pragma once
typedef enum { IT_CONSUMABLE, IT_WEAPON, IT_ARMOR, IT_KEY } ItemType;

typedef struct {
    int id;              // référençable via DB (data-driven)
    ItemType type;
    const char* name;    // pointeur vers data statique/chargée
    // effets simples :
    float hp_delta, mp_delta;   // potion…
    float atk_bonus, def_bonus; // arme/armure…
} ItemDef;

typedef struct {
    int item_id;   // référence à ItemDef dans la DB
    int qty;       // pile pour consommables
} ItemStack;