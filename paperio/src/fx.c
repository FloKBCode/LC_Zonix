// fx.c - Gestionnaire d'effets visuels côté client
#include <stdlib.h>
#include <math.h>
#include "../include/fx.h"

static FxParticle particles[MAX_FX];

void fx_init(void) {
    for (int i = 0; i < MAX_FX; i++)
        particles[i].active = 0;
}

void fx_clear(void) { fx_init(); }

// Cherche un slot libre
static FxParticle* alloc_fx(void) {
    for (int i = 0; i < MAX_FX; i++)
        if (!particles[i].active) return &particles[i];
    return NULL;
}

void fx_spawn(FxType type, float x, float y, SDL_Color color) {
    int count = 0;
    float decay = 0.04f;
    int size = 2;

    switch (type) {
        case FX_CAPTURE:      count=12; decay=0.03f; size=3; break;
        case FX_KILL:         count=20; decay=0.025f; size=4; break;
        case FX_DEATH:        count=16; decay=0.02f; size=3; break;
        case FX_POWERUP_PICK: count=8;  decay=0.05f; size=2; break;
        case FX_TRAIL_FLASH:  count=4;  decay=0.08f; size=2; break;
        case FX_COUNTDOWN:    count=6;  decay=0.06f; size=2; break;
        case FX_TERRITORY_GAIN: count=8; decay=0.04f; size=2; break;
        default: return;
    }

    for (int i = 0; i < count; i++) {
        FxParticle *p = alloc_fx();
        if (!p) return;

        float angle = ((float)(rand() % 360)) * 3.14159f / 180.0f;
        float speed = 0.5f + ((float)(rand() % 100)) / 50.0f;

        p->active     = 1;
        p->type       = type;
        p->x          = x;
        p->y          = y;
        p->vx         = cosf(angle) * speed;
        p->vy         = sinf(angle) * speed;
        p->life       = 1.0f;
        p->life_decay = decay + ((float)(rand()%20))/1000.0f;
        p->color      = color;
        p->color.a    = 255;
        p->size       = size;
    }
}

void fx_update(void) {
    for (int i = 0; i < MAX_FX; i++) {
        FxParticle *p = &particles[i];
        if (!p->active) continue;

        p->x    += p->vx;
        p->y    += p->vy;
        p->vy   += 0.05f; // gravité légère
        p->life -= p->life_decay;

        if (p->life <= 0.0f) p->active = 0;
    }
}

void fx_render(SDL_Renderer *ren) {
    for (int i = 0; i < MAX_FX; i++) {
        FxParticle *p = &particles[i];
        if (!p->active) continue;

        Uint8 alpha = (Uint8)(p->life * 255.0f);
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren, p->color.r, p->color.g, p->color.b, alpha);

        int sz = (int)(p->size * p->life) + 1;
        SDL_Rect r = { (int)p->x - sz/2, (int)p->y - sz/2, sz, sz };
        SDL_RenderFillRect(ren, &r);
    }
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
}
