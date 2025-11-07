#include "player/inventory.h"
#include "player/item.h"
#include <stddef.h>

int inv_use(Inventory* inv, int slot, /*out*/ ItemStack* used) {
    if (!inv) return 0;
    if (slot < 0 || slot >= INV_SLOTS) return 0;

    ItemStack* stack = &inv->slots[slot];

    // Empty slot if item_id == 0 or qty <= 0
    if (stack->item_id == 0 || stack->qty <= 0) return 0;

    const ItemDef* def = item_def(stack->item_id);
    if (!def) return 0;
    if (used) {
        *used = *stack;
    }

    // consumation des items
    if (def->consumable) {
        if (stack->qty > 1) {
            stack->qty -= 1;
        } else {
            // Clear the slot
            stack->item_id = 0;
            stack->qty = 0;
        }
    }

    // TODO: appliquer des effets à un joueur/personnage:
    //    - change prototype to take a Player* (et update header
    //    - add a function inv_apply_effect(Inventory*, int slot, Player*)
    // see what fit the best
    //

    return 1;
}
