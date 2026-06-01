// game.h - logique principale : update par tick, conditions de fin
#ifndef GAME_H
#define GAME_H

#include "types.h"

// Initialise l'etat de jeu complet : carte, joueurs, positions de depart
void game_init(GameState *gs, int player_count);

// Appele a chaque tick serveur :
//   1. Deplace chaque joueur vivant
//   2. Detecte collisions (trail ennemi, bord, trail propre)
//   3. Detecte retour sur territoire -> flood fill
//   4. Met a jour les scores
void game_update(GameState *gs);

// Renvoie 1 si le jeu est termine (1 seul joueur vivant, ou 0 joueur)
int  game_is_over(GameState *gs);

// Renvoie l'ID (1-4) du gagnant, ou 0 si match nul
int  game_winner(GameState *gs);

#endif
