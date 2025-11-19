#pragma once

#include "player/player.h"

#define WORLD_WIDTH  4
#define WORLD_HEIGHT 3
#define MAX_ZONES    12

typedef enum {
    ZONE_SURFACE,
    ZONE_REEF,
    ZONE_WRECK,
    ZONE_ALGAE,
    ZONE_CAVE,
    ZONE_ABYSS
} ZoneType;

typedef struct {
    ZoneType type;
    char name[30];
    int depth;
    int explored;
    int has_treasure;
    int creature_difficulty;
    char description[100];
} MapZone;

// j'ai renommé GameMap -> WorldMap pour éviter la confusion avec les "Map"
typedef struct {
    MapZone zones[WORLD_HEIGHT][WORLD_WIDTH];
    int player_x;
    int player_y;
} WorldMap;

void worldmap_init(WorldMap* map);
void worldmap_move_player(WorldMap* map, int dx, int dy, Player* player);
int  worldmap_can_move_to(WorldMap* map, int x, int y, Player* player);
void worldmap_explore_zone(WorldMap* map, int x, int y);
MapZone* worldmap_get_current_zone(WorldMap* map);
const char* worldmap_get_zone_name(ZoneType type);
const char* worldmap_get_zone_description(ZoneType type);
