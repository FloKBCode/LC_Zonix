// flood_fill.h - BFS qui colorie l'interieur d'une zone fermee
#ifndef FLOOD_FILL_H
#define FLOOD_FILL_H

#include "types.h"

// Quand un joueur ferme une boucle avec sa trainee :
// cette fonction remplit l'interieur de la boucle avec l'ID du joueur.
// Elle utilise un BFS depuis les bords de la carte (approche "inverser" :
// tout ce que le BFS depuis les bords NE peut PAS atteindre = interieur).
// Retourne le nombre de cases gagnees.
int flood_fill_capture(GameState *gs, int player_id);

#endif
