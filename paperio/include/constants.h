// constants.h - toutes les constantes du jeu, modifier ici uniquement
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GRID_W       80      // largeur de la grille en cases
#define GRID_H       60      // hauteur de la grille en cases
#define CELL_SIZE    10      // taille d'une case en pixels (base 2D)
#define MAX_PLAYERS   4      // nombre max de joueurs
#define SERVER_PORT  5555    // port TCP par defaut
#define TICK_RATE     80     // ms entre chaque update serveur (~12 fps)
#define WINDOW_W    (GRID_W * CELL_SIZE)   // 800px
#define WINDOW_H    (GRID_H * CELL_SIZE)   // 600px

// ------------------------------------------------------------------
// Rendu 2.5D : effet d'elevation par shadow offset
// Chaque case est un rectangle plein + une ombre decalee en bas/droite
// L'ombre donne l'illusion de volume sans SDL2_image ni SDL2_ttf
// ------------------------------------------------------------------
#define SHADOW_X     3       // decalage horizontal de l'ombre (pixels)
#define SHADOW_Y     3       // decalage vertical de l'ombre (pixels)
#define HEAD_LIFT    2       // elevation supplementaire de la tete du joueur

#endif
