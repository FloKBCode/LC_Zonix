#ifndef BOT_H
#define BOT_H

#include "types.h"

// Calcule la meilleure direction pour un bot
// Retourne la Direction choisie
Direction bot_think(GameState *gs, int player_id);

// Initialise l'état interne du bot pour un joueur
void bot_init(int player_id);

#endif
