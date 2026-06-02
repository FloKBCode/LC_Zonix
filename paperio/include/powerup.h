#ifndef POWERUP_H
#define POWERUP_H

#include "types.h"

// Initialise tous les power-ups (inactifs)
void powerup_init(GameState *gs);

// Appelé chaque tick : spawn aléatoire + expiration
void powerup_update(GameState *gs);

// Vérifie si un joueur marche sur un power-up et l'applique
void powerup_check_pickup(GameState *gs, int player_id);

// Applique l'effet d'une bombe autour d'une position
void powerup_apply_bomb(GameState *gs, int player_id, int x, int y);

#endif
