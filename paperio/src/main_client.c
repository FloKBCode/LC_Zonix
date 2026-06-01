// main_client.c - point d'entree du client : menus + boucle de jeu SDL2
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <winsock2.h>
#include "../include/client.h"
#include "../include/render.h"
#include "../include/menu.h"
#include "../include/game.h"
#include "../include/types.h"
#include "../include/constants.h"

static int key_to_dir(SDL_Keycode sym) {
    switch (sym) {
        case SDLK_UP:    case SDLK_z: case SDLK_w: return DIR_UP;
        case SDLK_DOWN:  case SDLK_s:              return DIR_DOWN;
        case SDLK_LEFT:  case SDLK_q: case SDLK_a: return DIR_LEFT;
        case SDLK_RIGHT: case SDLK_d:              return DIR_RIGHT;
        default: return -1;
    }
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    SDL_Window   *win = NULL;
    SDL_Renderer *ren = NULL;

    if (render_init(&win, &ren) < 0) {
        printf("[CLIENT] Erreur SDL2 : %s\n", SDL_GetError());
        return 1;
    }

    NetworkConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    strncpy(cfg.ip, "192.168.1.", sizeof(cfg.ip) - 1);
    cfg.port = SERVER_PORT;

    int connected = 0;
    int player_id = -1;

    while (!connected) {
        MenuResult res = menu_main(ren);

        if (res == MENU_RESULT_QUIT) {
            render_cleanup(win, ren);
            return 0;
        }

        if (res == MENU_RESULT_SERVER || res == MENU_RESULT_CLIENT) {
            MenuResult net_res = menu_network(ren, &cfg, 0);
            if (net_res == MENU_RESULT_BACK) continue;
            if (net_res == MENU_RESULT_QUIT) {
                render_cleanup(win, ren);
                return 0;
            }

            menu_connecting(ren, cfg.ip, cfg.port);

            if (init_client(cfg.ip, cfg.port) == 0) {
                player_id = client_get_player_id();
                connected = 1;
            } else {
                SDL_SetRenderDrawColor(ren, 15, 10, 10, 255);
                SDL_RenderClear(ren);
                render_string(ren, 200, WINDOW_H/2 - 10,
                              "CONNEXION ECHOUEE",
                              (SDL_Color){255, 80, 80, 255}, 2);
                render_string(ren, 200, WINDOW_H/2 + 20,
                              "VERIFIEZ L IP ET LE PORT",
                              (SDL_Color){180, 120, 120, 255}, 1);
                render_string(ren, 200, WINDOW_H/2 + 40,
                              "APPUIE SUR UNE TOUCHE",
                              (SDL_Color){120, 120, 120, 255}, 1);
                SDL_RenderPresent(ren);
                SDL_Event ev;
                int w = 1;
                while (w) {
                    while (SDL_PollEvent(&ev))
                        if (ev.type == SDL_KEYDOWN || ev.type == SDL_QUIT) w = 0;
                    SDL_Delay(16);
                }
            }
        }
    }

    printf("[CLIENT] Joueur %d connecte. Attente du debut...\n", player_id);

    // Attente du premier GameState valide (running=1)
    GameState gs;
    memset(&gs, 0, sizeof(gs));
    {
        SDL_Event ev;
        int waiting = 1;
        int anim = 0;
        while (waiting) {
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    disconnect_client();
                    render_cleanup(win, ren);
                    return 0;
                }
            }
            if (anim % 30 == 0)
                menu_waiting(ren, 0, 1);
            anim++;
            int r = receive_state(&gs);
            if (r > 0 && gs.running) waiting = 0;
            SDL_Delay(16);
        }
    }

    printf("[CLIENT] Partie demarree !\n");

    // Boucle de jeu
    SDL_Event ev;
    int running   = 1;
    int game_over = 0;
    int winner    = 0;
    Direction cur_dir = gs.players[player_id - 1].dir;
    Uint32 last_send  = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = 0;
            if (ev.type == SDL_KEYDOWN) {
                if (game_over && ev.key.keysym.sym == SDLK_SPACE) running = 0;
                if (ev.key.keysym.sym == SDLK_ESCAPE) running = 0;
                int dir = key_to_dir(ev.key.keysym.sym);
                if (dir >= 0) cur_dir = (Direction)dir;
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - last_send >= (Uint32)(TICK_RATE / 2)) {
            send_input(player_id, cur_dir);
            last_send = now;
        }

        GameState tmp;
        int r = receive_state(&tmp);
        if (r > 0) {
            gs = tmp;
            if (!gs.running && !game_over) {
                game_over = 1;
                winner    = game_winner(&gs);
            }
        } else if (r < 0) {
            printf("[CLIENT] Serveur deconnecte\n");
            running = 0;
            break;
        }

        render_draw(ren, &gs, player_id);
        if (game_over)
            render_game_over(ren, winner, &gs);

        SDL_Delay(8);
    }

    disconnect_client();
    render_cleanup(win, ren);
    return 0;
}