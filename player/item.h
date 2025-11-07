#pragma once
typedef enum { IT_CONSUMABLE, IT_WEAPON, IT_ARMOR, IT_KEY, IT_TRESOR } ItemType;

typedef struct {
    int id;              // référençable via DB (data-driven)
    ItemType type;
    const char* name;    // pointeur vers data statique/chargée
    // effets simples :
    int  stackable;      // 0/1
    int  max_stack;      // ex: x pour ressources, 1 pour arme par exemple
    int  consumable; 
    float hp_delta, mp_delta;   // +/- vie/mana/oxygène
    float defense, attack; // arme/armure
} ItemDef;

typedef struct {
    int item_id;   // référence à ItemDef dans la DB
    int qty;       // pile pour consommables
} ItemStack;

const ItemDef* item_def(int id);
size_t item_count(void);