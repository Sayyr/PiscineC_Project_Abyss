#include "item.h"
#include <stddef.h> //pour le size_t parce que franchement j'ai commencé mon jeu avec l'idée qu'il soit en 64bits
// et si j'utilise un unsigned int alors le jeu pourrait que être en 32bits, parce qu'on pourrait pas avoir
// plussieurs milliers ou millions d'éléments (jsp la limitation réelle en vrai, mais dans le doute, je prends size_t)

// test simple, plus tard en faire un JSON
static const ItemDef item_db[] = {

// rappel def : 
    // typedef struct {
    //     int id;
    //     ItemType type;
    //     const char* name;
    //     int stackable; 0=peut pas être stack
    //     int max_stack; 
    //     int unique; chaque objet ne peut être utilisé qu'une fois, à mettre sur les consommable
    //     float heal; comb ça rend de pv
    //     float mp_delta; comb ça rend de mana 
    //     float defense;
    //     float attack;
    // } ItemDef;
    {
        .id=0, .type=IT_CONSUMABLE, .name="empty",
        .stackable=0, .max_stack=0, .consumable=0,
        .hp_delta=0.0f, .mp_delta=0.0f,
        .defense=0.0f, .attack=0.0f
    },
    { 1, IT_CONSUMABLE, "petite Potion Rouge", 1, 5, 1, 25.0f, 0.0f, 0.0f, 0.0f },
    { 2, IT_WEAPON,     "Vieux Harpoon",       0, 1, 0,  0.0f, 0.0f, 0.0f, 2.0f },
    { 3, IT_WEAPON,     "super Harpoon",       0, 1, 0,  0.0f, 0.0f, 0.0f, 5.0f },
    { 4, IT_ARMOR,      "Plaque d'oricalque",  0, 1, 0,  0.0f, 0.0f, 4.0f, 0.0f },
    { 5, IT_KEY,        "Clé de Bronze",       0, 1, 1,  0.0f, 0.0f, 0.0f, 0.0f },
    { 6, IT_TRESOR,     "Pierre Précieuse",    1, 99, 1, 0.0f, 0.0f, 0.0f, 0.0f }
    // ajouter autant d'item que necessaire
};

const ItemDef* item_def(int id) {
    if (id < 0 || (size_t)id >= ARRAY_LEN(item_db)) return NULL;
    // id == index ('fin c'est une suppo, moins sûr de rien)
    return &item_db[id];
}

size_t item_count(void) {
    return ARRAY_LEN(item_db);
}