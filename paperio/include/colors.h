// colors.h - palette de couleurs 2.5D pour les 4 joueurs
#ifndef COLORS_H
#define COLORS_H

#include <SDL2/SDL.h>

// Chaque joueur a 4 couleurs :
//   territory = couleur principale de la case possedee (face du dessus)
//   shadow    = couleur de l'ombre portee (face avant/droite du "bloc")
//   trail     = couleur de la trainee (plus vive, semi-transparente visuellement)
//   head      = couleur de la tete (blanc brillant pour tous -> bien visible)
typedef struct {
    SDL_Color territory;
    SDL_Color shadow;
    SDL_Color trail;
    SDL_Color trail_shadow;
    SDL_Color head;
    SDL_Color head_shadow;
} PlayerColors;

static const PlayerColors PLAYER_COLORS[4] = {
    // Joueur 1 : Rouge
    {
        {210,  45,  45, 255},   // territoire
        {110,  15,  15, 255},   // ombre territoire
        {255, 110, 110, 255},   // trainee
        {150,  40,  40, 255},   // ombre trainee
        {255, 230, 230, 255},   // tete
        {180,  60,  60, 255},   // ombre tete
    },
    // Joueur 2 : Bleu
    {
        { 40, 110, 220, 255},
        { 10,  40, 110, 255},
        {110, 180, 255, 255},
        { 30,  80, 160, 255},
        {220, 235, 255, 255},
        { 40,  90, 180, 255},
    },
    // Joueur 3 : Vert
    {
        { 40, 190,  70, 255},
        { 10,  90,  25, 255},
        {110, 255, 140, 255},
        { 30, 130,  55, 255},
        {220, 255, 225, 255},
        { 30, 140,  55, 255},
    },
    // Joueur 4 : Orange
    {
        {220, 150,  30, 255},
        {110,  65,   5, 255},
        {255, 205,  90, 255},
        {160,  90,  10, 255},
        {255, 245, 200, 255},
        {180, 110,  20, 255},
    },
};

// Fond (cases libres) : gris fonce avec legere teinte bleue
#define COLOR_BG_TOP    ((SDL_Color){22, 24, 35, 255})
#define COLOR_BG_SHADOW ((SDL_Color){10, 11, 18, 255})

// Couleur du texte HUD (blanc casse)
#define COLOR_HUD ((SDL_Color){220, 220, 220, 255})

// Couleur du bord de grille
#define COLOR_GRID ((SDL_Color){30, 32, 45, 255})

#endif
