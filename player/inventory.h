#pragma once
#include "item.h"
#define INV_SLOTS 24

typedef struct {
    ItemStack slots[INV_SLOTS];
} Inventory;

int  inv_add(Inventory* inv, int item_id, int qty);
int  inv_remove(Inventory* inv, int item_id, int qty);
int  inv_use(Inventory* inv, int slot, /*out*/ItemStack* used); // renvoie l’item utilisé