#include <SDL2/SDL.h>
#include "constants.h"
#include "types.h"
#include "render.h"
#include "colors.h"

int render_init(SDL_Window **win, SDL_Renderer **ren) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return -1;

    *win = SDL_CreateWindow("Paper.io",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, 0);
    if (!*win) return -1;

    *ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);
    if (!*ren) return -1;

    return 0;
}

void render_draw(SDL_Renderer *ren, GameState *gs, int my_id) {
    SDL_SetRenderDrawColor(ren, 15, 15, 25, 255);
    SDL_RenderClear(ren);

    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            int owner = gs->map[y][x];
            if (owner == 0) continue;

            SDL_Color c = PLAYER_COLORS[owner - 1].territory;
            SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
            SDL_Rect cell = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            SDL_RenderFillRect(ren, &cell);
        }
    }

    for (int i = 0; i < gs->player_count; i++) {
        if (!gs->players[i].alive) continue;

        SDL_Color tc = PLAYER_COLORS[i].trail;
        SDL_SetRenderDrawColor(ren, tc.r, tc.g, tc.b, tc.a);
        for (int t = 0; t < gs->players[i].trail_len; t++) {
            SDL_Rect cell = {
                gs->players[i].trail[t][0] * CELL_SIZE,
                gs->players[i].trail[t][1] * CELL_SIZE,
                CELL_SIZE, CELL_SIZE
            };
            SDL_RenderFillRect(ren, &cell);
        }

        SDL_Color hc = PLAYER_COLORS[i].head;
        SDL_SetRenderDrawColor(ren, hc.r, hc.g, hc.b, hc.a);
        SDL_Rect head = {
            gs->players[i].x * CELL_SIZE,
            gs->players[i].y * CELL_SIZE,
            CELL_SIZE, CELL_SIZE
        };
        SDL_RenderFillRect(ren, &head);
    }

    SDL_RenderPresent(ren);
}

void render_cleanup(SDL_Window *win, SDL_Renderer *ren) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}