#ifndef GAME_H
#define GAME_H

#include "types.h"

void game_init(GameState *gs, int player_count, int grid_w, int grid_h);
void game_update(GameState *gs);
int  game_is_over(GameState *gs);
int  game_winner(GameState *gs);
void game_build_stats(GameState *gs, EndStats *stats);

#endif
