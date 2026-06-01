// player.h - deplacement, trail, mort, collision
#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

// Initialise un joueur a une position de depart et une direction
void player_init(Player *p, int x, int y, Direction dir);

// Deplace le joueur d'une case dans sa direction
// Met a jour le trail et verifie si retour sur territoire
// Retourne 1 si le joueur vient de fermer une boucle (declenchera flood fill)
int  player_move(GameState *gs, int player_id);

// Tue un joueur : efface sa trainee de la carte, met alive=0
void player_kill(GameState *gs, int player_id);

// Verifie si la case (nx, ny) appartient au trail du joueur player_id
int  player_trail_contains(Player *p, int nx, int ny);

// Convertit la trainee en territoire : applique l'ID sur la carte
void player_claim_trail(GameState *gs, int player_id);

// Efface la trainee d'un joueur (quand il meurt ou revient)
void player_clear_trail(GameState *gs, int player_id);

#endif
