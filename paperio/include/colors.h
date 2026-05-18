#ifndef COLORS_H
#define COLORS_H

#include <SDL2/SDL.h>

// Couleurs des 4 joueurs : territoire, trainee, tete
typedef struct {
    SDL_Color territory; // case possedee
    SDL_Color trail;     // trainee en cours
    SDL_Color head;      // tete du joueur
} PlayerColors;

static const PlayerColors PLAYER_COLORS[4] = {
    { {220, 50,  50,  255}, {255, 120, 120, 255}, {255, 255, 255, 255} }, // joueur 1 : rouge
    { {50,  120, 220, 255}, {120, 180, 255, 255}, {255, 255, 255, 255} }, // joueur 2 : bleu
    { {50,  200, 80,  255}, {120, 255, 150, 255}, {255, 255, 255, 255} }, // joueur 3 : vert
    { {220, 160, 50,  255}, {255, 210, 120, 255}, {255, 255, 255, 255} }, // joueur 4 : orange
};

// Couleur du fond (cases libres)
#define COLOR_BG (SDL_Color){15, 15, 25, 255}

#endif