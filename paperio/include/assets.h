// assets.h - Gestionnaire centralisé des assets graphiques
//
// REMPLACER LE LOGO / SPLASH :
//   Placer les fichiers dans assets/sprites/ et activer SDL2_image
//   Sinon : les écrans sont dessinés en SDL2 pur (render.c)
//
// Structure attendue :
//   assets/sprites/logo.bmp       -- logo ZONIX (optionnel, sinon pixel art)
//   assets/sprites/splash.bmp     -- splash screen (optionnel)
//   assets/sprites/icon.bmp       -- icône fenêtre
//   assets/sprites/pup_speed.bmp  -- icône power-up vitesse
//   assets/sprites/pup_shield.bmp -- icône power-up bouclier
//   assets/sprites/pup_bomb.bmp   -- icône power-up bombe
//   assets/sprites/pup_terr.bmp   -- icône power-up territoire

#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>

// Identifiants des sprites
typedef enum {
    SPRITE_LOGO = 0,
    SPRITE_SPLASH,
    SPRITE_ICON,
    SPRITE_PUP_SPEED,
    SPRITE_PUP_SHIELD,
    SPRITE_PUP_BOMB,
    SPRITE_PUP_TERRITORY,
    SPRITE_COUNT
} SpriteId;

// Charge tous les sprites disponibles (pas d'erreur si manquant)
void assets_init(SDL_Renderer *ren);
void assets_shutdown(void);

// Renvoie la texture, ou NULL si non chargée
SDL_Texture* assets_get(SpriteId id);

// Dessine un sprite si dispo, sinon ne fait rien (fallback au code SDL)
void assets_draw(SDL_Renderer *ren, SpriteId id, int x, int y, int w, int h);

#endif
