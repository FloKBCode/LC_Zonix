#include <SDL2/SDL.h>
#include <stdio.h>
#include "constants.h"
#include "types.h"
#include "render.h"

int main(int argc, char *argv[]) {
    SDL_Window   *win = NULL;
    SDL_Renderer *ren = NULL;

    if (render_init(&win, &ren) != 0) {
        printf("Erreur init SDL2\n");
        return 1;
    }

    GameState gs = {0};
    gs.player_count = 1;
    gs.running = 1;

    gs.players[0].x = GRID_W / 2;
    gs.players[0].y = GRID_H / 2;
    gs.players[0].alive = 1;
    gs.players[0].dir = DIR_RIGHT;

    int my_id = 0;

    SDL_Event e;
    while (gs.running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) gs.running = 0;

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    case SDLK_z: gs.players[0].dir = DIR_UP;    break;
                    case SDLK_DOWN:  case SDLK_s: gs.players[0].dir = DIR_DOWN;  break;
                    case SDLK_LEFT:  case SDLK_q: gs.players[0].dir = DIR_LEFT;  break;
                    case SDLK_RIGHT: case SDLK_d: gs.players[0].dir = DIR_RIGHT; break;
                    case SDLK_ESCAPE: gs.running = 0; break;
                }
            }
        }

        if (gs.players[0].dir == DIR_UP)    gs.players[0].y--;
        if (gs.players[0].dir == DIR_DOWN)  gs.players[0].y++;
        if (gs.players[0].dir == DIR_LEFT)  gs.players[0].x--;
        if (gs.players[0].dir == DIR_RIGHT) gs.players[0].x++;

        if (gs.players[0].x < 0) gs.players[0].x = 0;
        if (gs.players[0].y < 0) gs.players[0].y = 0;
        if (gs.players[0].x >= GRID_W) gs.players[0].x = GRID_W - 1;
        if (gs.players[0].y >= GRID_H) gs.players[0].y = GRID_H - 1;

        if (gs.players[0].trail_len < GRID_W * GRID_H) {
            gs.players[0].trail[gs.players[0].trail_len][0] = gs.players[0].x;
            gs.players[0].trail[gs.players[0].trail_len][1] = gs.players[0].y;
            gs.players[0].trail_len++;
        }

        render_draw(ren, &gs, my_id);
        SDL_Delay(TICK_RATE);
    }

    render_cleanup(win, ren);
    return 0;
}