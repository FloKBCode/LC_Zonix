#ifndef LOBBY_H
#define LOBBY_H

#include <SDL2/SDL.h>
#include "types.h"

// Infos saisies par le joueur dans le lobby
typedef struct {
    char pseudo[16];
    int  color_idx;
    int  grid_size;   // 0=petit(60x45) 1=normal(80x60) 2=grand(100x75)
    Theme theme;
} LobbyInfo;

// Ecran lobby côté client : saisie pseudo, couleur, taille, thème
// Retourne 0 si OK, -1 si quit
int lobby_screen(SDL_Renderer *ren, LobbyInfo *info, int player_id);

// Ecran classement/stats en fin de partie
void lobby_show_stats(SDL_Renderer *ren, EndStats *stats, int my_id, Theme theme);

#endif
