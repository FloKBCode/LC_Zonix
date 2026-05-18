#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "constants.h"
#include "types.h"

int render_init(SDL_Window **win, SDL_Renderer **ren);
void render_draw(SDL_Renderer *ren, GameState *gs, int my_id);
void render_cleanup(SDL_Window *win, SDL_Renderer *ren);

#endif