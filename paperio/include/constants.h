#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GRID_W        80
#define GRID_H        60
#define CELL_SIZE     10
#define MAX_PLAYERS    4
#define SERVER_PORT   5555
#define TICK_RATE      80
#define WINDOW_W      (GRID_W * CELL_SIZE)
#define WINDOW_H      (GRID_H * CELL_SIZE)
#define SHADOW_X       3
#define SHADOW_Y       3
#define HEAD_LIFT      2

// Countdown avant debut (ticks)
#define COUNTDOWN_TICKS 37   // ~3 secondes a 80ms/tick

// Bouclier spawn (ticks)
#define SPAWN_SHIELD_TICKS 37

// Power-ups
#define POWERUP_LIFETIME_TICKS 100   // ~8s avant disparition
#define POWERUP_SPAWN_INTERVAL  60   // 1 nouveau toutes les ~5s
#define SPEED_BOOST_TICKS       62   // ~5s
#define SHIELD_TICKS            37   // ~3s

// Nombre de couleurs disponibles dans la palette joueur
#define COLOR_PALETTE_SIZE 12

#endif
