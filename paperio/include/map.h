#ifndef MAP_H
#define MAP_H

#include "types.h"

void map_init(GameState *gs);
int  map_get(GameState *gs, int x, int y);
void map_set(GameState *gs, int x, int y, int owner);
int  map_in_bounds(int x, int y);
int  map_in_bounds_gs(GameState *gs, int x, int y);  // taille dynamique
void map_calc_scores(GameState *gs);

#endif
