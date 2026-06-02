// fx.h - Gestionnaire centralisé d'effets visuels
//
// AJOUTER UN NOUVEL EFFET :
//   1. Ajouter une valeur dans FxType
//   2. Ajouter un case dans fx_update() (src/fx.c)
//   3. Ajouter un case dans fx_render() (src/fx.c)
//   4. Appeler fx_spawn(...) depuis le code de jeu
//
// Les effets sont des particules éphémères : position, vélocité, durée, couleur.
// Ils ne sont que côté client (render.c les appelle).

#ifndef FX_H
#define FX_H

#include <SDL2/SDL.h>
#include "types.h"

#define MAX_FX 128

typedef enum {
    FX_NONE = 0,
    FX_CAPTURE,       // burst de particules quand on capture une zone
    FX_KILL,          // explosion quand on tue un ennemi
    FX_DEATH,         // dispersion quand on meurt
    FX_POWERUP_PICK,  // scintillement ramassage power-up
    FX_TRAIL_FLASH,   // éclair sur le trail
    FX_COUNTDOWN,     // pulse sur les chiffres du countdown
    FX_TERRITORY_GAIN,// vague d'expansion territoire
} FxType;

typedef struct {
    FxType   type;
    int      active;
    float    x, y;       // position en pixels
    float    vx, vy;     // vélocité
    float    life;       // 0.0 = mort, 1.0 = neuf
    float    life_decay; // soustrait à chaque frame
    SDL_Color color;
    int      size;       // taille en pixels
} FxParticle;

// --- API FX ---
void fx_init(void);
void fx_spawn(FxType type, float x_px, float y_px, SDL_Color color);
void fx_update(void);       // appelé dans render_draw avant SDL_RenderPresent
void fx_render(SDL_Renderer *ren);
void fx_clear(void);        // vide tous les effets (changement de scène)

#endif
