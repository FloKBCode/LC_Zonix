// types.h - structs partagees entre serveur et client
#ifndef TYPES_H
#define TYPES_H

#include "constants.h"

// Thèmes visuels disponibles
typedef enum {
    THEME_DARK = 0,
    THEME_RETRO,
    THEME_SYNTHWAVE,
    THEME_MINIMAL,
    THEME_GIRLY,
    THEME_COUNT
} Theme;

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

// Types de power-up
typedef enum {
    PUP_NONE = 0,
    PUP_SPEED,      // vitesse x2 pendant 5s
    PUP_SHIELD,     // invincible 3s
    PUP_BOMB,       // detruit le territoire autour
    PUP_TERRITORY   // capture instantanee d'un carre 5x5
} PowerUpType;

// Un power-up sur la carte
typedef struct {
    int x, y;
    PowerUpType type;
    int active;         // 1 = visible sur la carte
    int lifetime;       // ticks restants avant disparition
} PowerUp;

typedef struct {
    int x, y;
    Direction dir;
    int alive;
    int score;
    int trail[GRID_W * GRID_H][2];
    int trail_len;
    int returning;
    // Effets actifs
    int shield_ticks;   // ticks d'invincibilite restants
    int speed_ticks;    // ticks de boost vitesse restants
    int move_skip;      // compteur pour gerer la vitesse (0=normal, skip=lent)
    // Personnalisation
    char pseudo[16];
    int color_idx;      // index dans la palette de couleurs
    // Stats de la partie
    int kills;          // joueurs tues
    int captures;       // nombre de captures realisees
    int max_territory;  // territoire max atteint
} Player;

#define MAX_POWERUPS 8

typedef struct {
    Player players[MAX_PLAYERS];
    int map[GRID_H][GRID_W];
    int player_count;
    int running;
    int ticks;
    int countdown;      // ticks de countdown avant debut (0 = jeu lance)
    PowerUp powerups[MAX_POWERUPS];
    int grid_w;         // taille effective (peut etre reduite)
    int grid_h;
    Theme theme;        // theme visuel choisi
} GameState;

// Paquet input client -> serveur
typedef struct {
    int player_id;
    Direction dir;
} Packet;

// Paquet de lobby : envoi des infos perso avant la partie
typedef struct {
    int  player_id;
    char pseudo[16];
    int  color_idx;
} LobbyPacket;

// Stats fin de partie (envoyees par le serveur)
typedef struct {
    int winner_id;
    int scores[MAX_PLAYERS];
    int kills[MAX_PLAYERS];
    int captures[MAX_PLAYERS];
    int max_territory[MAX_PLAYERS];
    char pseudos[MAX_PLAYERS][16];
    int  grid_w;
    int  grid_h;
} EndStats;

#endif