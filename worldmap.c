#include "worldmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const char* worldmap_get_zone_name(ZoneType type) {
    switch (type) {
        case ZONE_SURFACE: return "Surface";
        case ZONE_REEF:    return "Recif Corallien";
        case ZONE_WRECK:   return "Epave";
        case ZONE_ALGAE:   return "Foret d'Algues";
        case ZONE_CAVE:    return "Grotte Sous-Marine";
        case ZONE_ABYSS:   return "Fosse Abyssale";
        default:           return "Zone Inconnue";
    }
}

const char* worldmap_get_zone_description(ZoneType type) {
    switch (type) {
        case ZONE_SURFACE: return "Zone securisee pour repos et sauvegarde";
        case ZONE_REEF:    return "Creatures faciles, equipements de base";
        case ZONE_WRECK:   return "Tresors, creatures moyennes";
        case ZONE_ALGAE:   return "Labyrinthe naturel, creatures cachees";
        case ZONE_CAVE:    return "Zone securisee, cristaux d'oxygene";
        case ZONE_ABYSS:   return "Creatures legendaires, dangers extremes";
        default:           return "Description inconnue";
    }
}

void worldmap_init(WorldMap* map) {
    srand((unsigned)time(NULL));

    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            ZoneType type;
            int depth;

            if (y == 0) {
                if (x == 0) {
                    type = ZONE_SURFACE;
                    depth = 0;
                } else {
                    type = ZONE_REEF;
                    depth = 50 + (x * 25);
                }
            } else if (y == 1) {
                type = (ZoneType)(ZONE_WRECK + (rand() % 2));
                depth = 150 + (x * 50);
            } else {
                type = (ZoneType)(ZONE_CAVE + (rand() % 2));
                depth = 300 + (x * 100);
            }

            MapZone* zone = &map->zones[y][x];
            zone->type = type;
            strcpy(zone->name, worldmap_get_zone_name(type));
            zone->depth = depth;
            zone->explored = (y == 0 && x == 0);
            zone->has_treasure = (rand() % 100 < 30);
            zone->creature_difficulty = depth / 50;
            strcpy(zone->description, worldmap_get_zone_description(type));
        }
    }

    map->player_x = 0;
    map->player_y = 0;
}

int worldmap_can_move_to(WorldMap* map, int x, int y, Player* player) {
    if (x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT) {
        printf("Hors des limites de la carte!\n");
        return 0;
    }

    MapZone* zone = &map->zones[y][x];

    int min_oxygen = zone->depth / 50;
    if (player->stats.oxygen < min_oxygen) {
        printf("Oxygene insuffisant pour cette profondeur! (minimum: %d)\n", min_oxygen);
        return 0;
    }

    return 1;
}

void worldmap_explore_zone(WorldMap* map, int x, int y) {
    map->zones[y][x].explored = 1;
}

MapZone* worldmap_get_current_zone(WorldMap* map) {
    return &map->zones[map->player_y][map->player_x];
}

void worldmap_move_player(WorldMap* map, int dx, int dy, Player* player) {
    int new_x = map->player_x + dx;
    int new_y = map->player_y + dy;

    if (worldmap_can_move_to(map, new_x, new_y, player)) {
        map->player_x = new_x;
        map->player_y = new_y;

        MapZone* zone = &map->zones[new_y][new_x];
        player->stats.depth = zone->depth;

        worldmap_explore_zone(map, new_x, new_y);

        int oxygen_cost = abs(dx) + abs(dy) + (zone->depth / 100);
        if (oxygen_cost < 1) oxygen_cost = 1;
        player_use_oxygen(player, oxygen_cost);

        printf("Deplacement vers: %s (%dm)\n", zone->name, zone->depth);
        printf("Consommation d'oxygene: -%d\n", oxygen_cost);

        if (zone->has_treasure && !zone->explored) {
            printf("Vous trouvez un tresor! +10 perles.\n");
            player->stats.pearls += 10;
            zone->has_treasure = 0;
        }
    } else {
        printf("Deplacement impossible!\n");
    }
}
