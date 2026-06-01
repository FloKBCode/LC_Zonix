#ifndef COLORS_H
#define COLORS_H

#include <SDL2/SDL.h>
#include "constants.h"
#include "types.h"

// Palette de couleurs choisissables par les joueurs (12 couleurs)
typedef struct {
    const char   *name;
    SDL_Color     territory;
    SDL_Color     shadow;
    SDL_Color     trail;
    SDL_Color     trail_shadow;
    SDL_Color     head;
    SDL_Color     head_shadow;
} PaletteColor;

extern const PaletteColor COLOR_PALETTE[COLOR_PALETTE_SIZE];

// Couleurs des thèmes (fond, grille, HUD)
typedef struct {
    SDL_Color bg_top;
    SDL_Color bg_shadow;
    SDL_Color grid;
    SDL_Color hud_bg;
    SDL_Color hud_text;
    SDL_Color countdown;
} ThemeColors;

extern const ThemeColors THEMES[THEME_COUNT];

// Couleurs des power-ups
typedef struct { SDL_Color top; SDL_Color shadow; const char *icon; } PupColor;
extern const PupColor POWERUP_COLORS[5]; // index = PowerUpType

#endif
