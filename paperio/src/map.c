// map.c - acces et modification du tableau 2D de la carte
#include <string.h>
#include "../include/map.h"
#include "../include/constants.h"

void map_init(GameState *gs) {
    memset(gs->map, 0, sizeof(gs->map));
}

int map_get(GameState *gs, int x, int y) {
    if (!map_in_bounds(x, y)) return -1;
    return gs->map[y][x];
}

void map_set(GameState *gs, int x, int y, int owner) {
    if (!map_in_bounds(x, y)) return;
    gs->map[y][x] = owner;
}

int map_in_bounds(int x, int y) {
    return (x >= 0 && x < GRID_W && y >= 0 && y < GRID_H);
}

void map_calc_scores(GameState *gs) {
    // Remet les scores a zero
    for (int i = 0; i < gs->player_count; i++)
        gs->players[i].score = 0;

    // Compte les cases par proprietaire
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            int owner = gs->map[y][x];
            if (owner >= 1 && owner <= MAX_PLAYERS)
                gs->players[owner - 1].score++;
        }
    }
}
