// constants.h - toutes les constantes du jeu, modifier ici uniquement
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GRID_W      80      // largeur de la grille en cases
#define GRID_H      60      // hauteur de la grille en cases
#define CELL_SIZE   10      // taille d'une case en pixels
#define MAX_PLAYERS  4      // nombre max de joueurs
#define SERVER_PORT  5555   // port TCP par defaut
#define TICK_RATE    100    // ms entre chaque update du jeu (10 fps)
#define WINDOW_W    (GRID_W * CELL_SIZE)   // 800px
#define WINDOW_H    (GRID_H * CELL_SIZE)   // 600px

#endif
