// map.h - acces et modification du tableau 2D de la carte
#ifndef MAP_H
#define MAP_H

#include "types.h"

// Initialise la carte : tous les blocs a 0 (libre)
void map_init(GameState *gs);

// Renvoie l'ID du proprietaire d'une case (0 = libre)
int  map_get(GameState *gs, int x, int y);

// Pose l'ID d'un joueur sur une case (0 pour effacer)
void map_set(GameState *gs, int x, int y, int owner);

// Verifie si (x,y) est dans les limites de la grille
int  map_in_bounds(int x, int y);

// Calcule le score de chaque joueur (nb de cases possedees)
void map_calc_scores(GameState *gs);

#endif
