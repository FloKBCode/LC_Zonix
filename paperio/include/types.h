// types.h - structs partagees entre serveur et client
#ifndef TYPES_H
#define TYPES_H

#include "constants.h"

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

typedef struct {
    int x, y;           // position sur la grille
    Direction dir;      // direction actuelle
    int alive;          // 1 = vivant, 0 = mort
    int score;          // % de territoire possede
    int trail[GRID_W * GRID_H][2]; // cases de la trainee [x][y]
    int trail_len;      // nombre de cases dans la trainee
} Player;

typedef struct {
    Player players[MAX_PLAYERS];
    int map[GRID_H][GRID_W]; // 0=libre, 1-4=ID proprietaire
    int player_count;
    int running;
} GameState;

// paquet envoye par le client au serveur : juste la direction
typedef struct {
    int player_id;
    Direction dir;
} Packet;

#endif
