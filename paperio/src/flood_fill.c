// flood_fill.c - BFS depuis les bords — FIX: utilise grid_w/grid_h dynamiques
#include <string.h>
#include "../include/flood_fill.h"
#include "../include/map.h"
#include "../include/constants.h"

static int bfs_queue[GRID_W * GRID_H][2];
static int visited[GRID_H][GRID_W];
static int barrier[GRID_H][GRID_W];

int flood_fill_capture(GameState *gs, int player_id) {
    int gw = gs->grid_w > 0 ? gs->grid_w : GRID_W;
    int gh = gs->grid_h > 0 ? gs->grid_h : GRID_H;

    memset(barrier, 0, sizeof(barrier));
    memset(visited, 0, sizeof(visited));

    // Territoire du joueur = barrière
    for (int y = 0; y < gh; y++)
        for (int x = 0; x < gw; x++)
            if (gs->map[y][x] == player_id)
                barrier[y][x] = 1;

    // Trail du joueur = barrière
    Player *p = &gs->players[player_id-1];
    for (int t = 0; t < p->trail_len; t++)
        barrier[ p->trail[t][1] ][ p->trail[t][0] ] = 1;

    // BFS depuis les bords
    int head = 0, tail = 0;
    for (int x = 0; x < gw; x++) {
        if (!barrier[0][x] && !visited[0][x]) {
            visited[0][x] = 1;
            bfs_queue[tail][0] = x; bfs_queue[tail][1] = 0; tail++;
        }
        if (!barrier[gh-1][x] && !visited[gh-1][x]) {
            visited[gh-1][x] = 1;
            bfs_queue[tail][0] = x; bfs_queue[tail][1] = gh-1; tail++;
        }
    }
    for (int y = 0; y < gh; y++) {
        if (!barrier[y][0] && !visited[y][0]) {
            visited[y][0] = 1;
            bfs_queue[tail][0] = 0; bfs_queue[tail][1] = y; tail++;
        }
        if (!barrier[y][gw-1] && !visited[y][gw-1]) {
            visited[y][gw-1] = 1;
            bfs_queue[tail][0] = gw-1; bfs_queue[tail][1] = y; tail++;
        }
    }

    int dx[4] = {1,-1,0,0}, dy[4] = {0,0,1,-1};
    while (head < tail) {
        int cx = bfs_queue[head][0], cy = bfs_queue[head][1];
        head++;
        for (int d = 0; d < 4; d++) {
            int nx = cx+dx[d], ny = cy+dy[d];
            if (nx<0||nx>=gw||ny<0||ny>=gh) continue;
            if (visited[ny][nx] || barrier[ny][nx]) continue;
            visited[ny][nx] = 1;
            bfs_queue[tail][0] = nx; bfs_queue[tail][1] = ny; tail++;
        }
    }

    int gained = 0;
    for (int y = 0; y < gh; y++)
        for (int x = 0; x < gw; x++)
            if (!visited[y][x] && !barrier[y][x]) {
                gs->map[y][x] = player_id;
                gained++;
            }
    return gained;
}
