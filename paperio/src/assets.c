// assets.c - Chargement optionnel des sprites (BMP sans SDL2_image)
#include <stdio.h>
#include "../include/assets.h"

static SDL_Texture *textures[SPRITE_COUNT] = {0};
static SDL_Renderer *_ren = NULL;

static const char *SPRITE_PATHS[SPRITE_COUNT] = {
    "assets/sprites/logo.bmp",
    "assets/sprites/splash.bmp",
    "assets/sprites/icon.bmp",
    "assets/sprites/pup_speed.bmp",
    "assets/sprites/pup_shield.bmp",
    "assets/sprites/pup_bomb.bmp",
    "assets/sprites/pup_territory.bmp",
};

void assets_init(SDL_Renderer *ren) {
    _ren = ren;
    for (int i = 0; i < SPRITE_COUNT; i++) {
        SDL_Surface *surf = SDL_LoadBMP(SPRITE_PATHS[i]);
        if (surf) {
            textures[i] = SDL_CreateTextureFromSurface(ren, surf);
            SDL_FreeSurface(surf);
            printf("[ASSETS] Charge : %s\n", SPRITE_PATHS[i]);
        }
        // Pas d'erreur si le fichier manque : fallback automatique
    }
}

void assets_shutdown(void) {
    for (int i = 0; i < SPRITE_COUNT; i++)
        if (textures[i]) SDL_DestroyTexture(textures[i]);
}

SDL_Texture* assets_get(SpriteId id) {
    if (id >= SPRITE_COUNT) return NULL;
    return textures[id];
}

void assets_draw(SDL_Renderer *ren, SpriteId id, int x, int y, int w, int h) {
    SDL_Texture *t = assets_get(id);
    if (!t) return;
    SDL_Rect dst = {x, y, w, h};
    SDL_RenderCopy(ren, t, NULL, &dst);
}
