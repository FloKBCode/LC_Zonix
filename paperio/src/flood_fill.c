// flood_fill.c - BFS depuis les bords pour trouver l'exterieur
// Tout ce que le BFS ne peut pas atteindre = interieur a capturer
#include <string.h>
#include "../include/flood_fill.h"
#include "../include/map.h"
#include "../include/constants.h"

// File BFS locale (taille max = toute la grille)
static int bfs_queue[GRID_W * GRID_H][2];

// Grille "visite" locale (evite d'allouer dynamiquement)
static int visited[GRID_H][GRID_W];

// Grille "est une barriere" : cases du territoire OU du trail du joueur
static int barrier[GRID_H][GRID_W];

int flood_fill_capture(GameState *gs, int player_id) {
    // --- 1. Construire la grille barrier ---
    // Une case est une barriere si elle appartient au joueur (territoire ou trail)
    memset(barrier, 0, sizeof(barrier));
    memset(visited, 0, sizeof(visited));

    // Territoire existant du joueur
    for (int y = 0; y < GRID_H; y++)
        for (int x = 0; x < GRID_W; x++)
            if (gs->map[y][x] == player_id)
                barrier[y][x] = 1;

    // Trail actuel du joueur
    Player *p = &gs->players[player_id - 1];
    for (int t = 0; t < p->trail_len; t++)
        barrier[ p->trail[t][1] ][ p->trail[t][0] ] = 1;

    // --- 2. BFS depuis tous les bords de la grille ---
    // Les cases de bord non-barrieres sont l'"exterieur"
    int head = 0, tail = 0;

    // Ligne du haut et du bas
    for (int x = 0; x < GRID_W; x++) {
        if (!barrier[0][x] && !visited[0][x]) {
            visited[0][x] = 1;
            bfs_queue[tail][0] = x;
            bfs_queue[tail][1] = 0;
            tail++;
        }
        if (!barrier[GRID_H-1][x] && !visited[GRID_H-1][x]) {
            visited[GRID_H-1][x] = 1;
            bfs_queue[tail][0] = x;
            bfs_queue[tail][1] = GRID_H - 1;
            tail++;
        }
    }
    // Colonne gauche et droite
    for (int y = 0; y < GRID_H; y++) {
        if (!barrier[y][0] && !visited[y][0]) {
            visited[y][0] = 1;
            bfs_queue[tail][0] = 0;
            bfs_queue[tail][1] = y;
            tail++;
        }
        if (!barrier[y][GRID_W-1] && !visited[y][GRID_W-1]) {
            visited[y][GRID_W-1] = 1;
            bfs_queue[tail][0] = GRID_W - 1;
            bfs_queue[tail][1] = y;
            tail++;
        }
    }

    // BFS : propagation dans les 4 directions
    int dx[4] = { 1, -1, 0, 0 };
    int dy[4] = { 0,  0, 1,-1 };

    while (head < tail) {
        int cx = bfs_queue[head][0];
        int cy = bfs_queue[head][1];
        head++;

        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d];
            int ny = cy + dy[d];
            if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H) continue;
            if (visited[ny][nx] || barrier[ny][nx]) continue;
            visited[ny][nx] = 1;
            bfs_queue[tail][0] = nx;
            bfs_queue[tail][1] = ny;
            tail++;
        }
    }

    // --- 3. Capturer l'interieur ---
    // Toute case non-visitee par le BFS ET non-barriere est a l'interieur
    int gained = 0;
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (!visited[y][x] && !barrier[y][x]) {
                // Case interieure : elle appartient maintenant au joueur
                // Si elle appartenait a un autre joueur, il la perd
                gs->map[y][x] = player_id;
                gained++;
            }
        }
    }

    return gained;
}
