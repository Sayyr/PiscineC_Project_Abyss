#pragma once
#include <stdbool.h>
// #ifndef MAP_H
// #define MAP_H

// #include "player/player.h"

// #define MAP_WIDTH 4
// #define MAP_HEIGHT 3
// #define MAX_ZONES 12

typedef struct Map {
    int w;
    int h;
    const char* data;
} Map;

// typedef enum {
//     ZONE_SURFACE,
//     ZONE_REEF,
//     ZONE_WRECK,
//     ZONE_ALGAE,
//     ZONE_CAVE,
//     ZONE_ABYSS
// } ZoneType;

// typedef struct {
//     ZoneType type;
//     char name[30];
//     int depth;
//     int explored;
//     int has_treasure;
//     int creature_difficulty;
//     char description[100];
// } MapZone;

// typedef struct {
//     MapZone zones[MAP_HEIGHT][MAP_WIDTH];
//     int player_x;
//     int player_y;
// } GameMap;

bool map_is_wall(const Map* m, int tx, int ty);
bool has_line_of_sight(const Map* map, float x0, float y0, float x1, float y1);
// void map_init(GameMap* map);
// void map_move_player(GameMap* map, int dx, int dy, Player* player);
// int map_can_move_to(GameMap* map, int x, int y, Player* player);
// void map_explore_zone(GameMap* map, int x, int y);
// MapZone* map_get_current_zone(GameMap* map);
// const char* get_zone_name(ZoneType type);
// const char* get_zone_description(ZoneType type);

// #endif