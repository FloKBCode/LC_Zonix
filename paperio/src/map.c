// map.c
#include <string.h>
#include "../include/map.h"
#include "../include/constants.h"

void map_init(GameState *gs) {
    memset(gs->map, 0, sizeof(gs->map));
}

int map_get(GameState *gs, int x, int y) {
    if (!map_in_bounds_gs(gs, x, y)) return -1;
    return gs->map[y][x];
}

void map_set(GameState *gs, int x, int y, int owner) {
    if (!map_in_bounds_gs(gs, x, y)) return;
    gs->map[y][x] = owner;
}

// Version statique (pour flood_fill qui travaille sur GRID_W/H max)
int map_in_bounds(int x, int y) {
    return (x >= 0 && x < GRID_W && y >= 0 && y < GRID_H);
}

// Version dynamique (taille effective de la grille)
int map_in_bounds_gs(GameState *gs, int x, int y) {
    int gw = gs->grid_w > 0 ? gs->grid_w : GRID_W;
    int gh = gs->grid_h > 0 ? gs->grid_h : GRID_H;
    return (x >= 0 && x < gw && y >= 0 && y < gh);
}

void map_calc_scores(GameState *gs) {
    for (int i = 0; i < gs->player_count; i++)
        gs->players[i].score = 0;
    int gw = gs->grid_w > 0 ? gs->grid_w : GRID_W;
    int gh = gs->grid_h > 0 ? gs->grid_h : GRID_H;
    for (int y = 0; y < gh; y++)
        for (int x = 0; x < gw; x++) {
            int o = gs->map[y][x];
            if (o >= 1 && o <= MAX_PLAYERS)
                gs->players[o-1].score++;
        }
}
