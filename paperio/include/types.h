#ifndef TYPES_H
#define TYPES_H

#include "constants.h"

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

typedef struct {
    int x, y;
    Direction dir;
    int alive;
    int score;
    int trail[GRID_W * GRID_H][2];
    int trail_len;
} Player;

typedef struct {
    Player players[MAX_PLAYERS];
    int map[GRID_H][GRID_W];
    int player_count;
    int running;
} GameState;

typedef struct {
    int player_id;
    Direction dir;
} Packet;

#endif