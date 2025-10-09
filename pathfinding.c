#include "game.h"
#include "pathfinding.h"
#include <stdlib.h>

int heuristic(int x0, int y0, int x1, int y1) {
    return abs(x1 - x0) + abs(y1 - y0);
}

int astar(const Map* map, int sx, int sy, int ex, int ey, Node path[MAX_PATH]) {
    int closed[64][64] = {0};
    Node open[MAX_PATH];
    int gscore[64][64] = {0};
    int came_from[64][64][2] = {{{-1, -1}}};
    int open_len = 0;
    open[open_len++] = (Node){sx, sy};
    gscore[sx][sy] = 0;

    while (open_len > 0) {
        int best = 0, best_f = 99999;
        for (int i = 0; i < open_len; ++i) {
            int f = gscore[open[i].x][open[i].y] + heuristic(open[i].x, open[i].y, ex, ey);
            if (f < best_f) { best_f = f; best = i; }
        }
        Node cur = open[best];
        open[best] = open[--open_len];

        if (cur.x == ex && cur.y == ey) {
            int len = 0;
            Node n = cur;
            while (!(n.x == sx && n.y == sy) && len < MAX_PATH) {
                path[len++] = n;
                int px = came_from[n.x][n.y][0];
                int py = came_from[n.x][n.y][1];
                n.x = px; n.y = py;
            }
            for (int i = 0; i < len/2; ++i) {
                Node tmp = path[i]; path[i] = path[len-1-i]; path[len-1-i] = tmp;
            }
            return len;
        }

        closed[cur.x][cur.y] = 1;
        const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
        for (int d = 0; d < 4; ++d) {
            int nx = cur.x + dx[d], ny = cur.y + dy[d];
            if (nx < 0 || ny < 0 || nx >= map->w || ny >= map->h) continue;
            if (map_is_wall(map, nx, ny) || closed[nx][ny]) continue;
            int tentative_g = gscore[cur.x][cur.y] + 1;
            if (!gscore[nx][ny] || tentative_g < gscore[nx][ny]) {
                gscore[nx][ny] = tentative_g;
                came_from[nx][ny][0] = cur.x;
                came_from[nx][ny][1] = cur.y;
                open[open_len++] = (Node){nx, ny};
                if (open_len >= MAX_PATH) break;
            }
        }
    }
    return 0;
}