#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <string.h>
#include "types.h"

#define GLYPH_ADV 8

static inline int text_w(const char *s, int scale) {
    if (!s) return 0;
    return (int)strlen(s) * GLYPH_ADV * scale;
}

int  render_init(SDL_Window **win, SDL_Renderer **ren);
void render_draw(SDL_Renderer *ren, GameState *gs, int my_id);
void render_cleanup(SDL_Window *win, SDL_Renderer *ren);
void render_block_25d(SDL_Renderer *ren, int x_px, int y_px, int sz, int lift,
                      SDL_Color top_color, SDL_Color shadow_color);
void render_char(SDL_Renderer *ren, int x, int y, char c, SDL_Color col, int scale);
void render_string(SDL_Renderer *ren, int x, int y, const char *str, SDL_Color col, int scale);
void render_hud(SDL_Renderer *ren, GameState *gs, int my_id);
void render_game_over(SDL_Renderer *ren, int winner_id, GameState *gs);

#endif