// main_client.c - avec sons SDL2 natifs
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <winsock2.h>
#include "../include/client.h"
#include "../include/render.h"
#include "../include/menu.h"
#include "../include/lobby.h"
#include "../include/game.h"
#include "../include/audio.h"
#include "../include/types.h"
#include "../include/constants.h"

static int key_to_dir(SDL_Keycode sym) {
    switch(sym){
        case SDLK_UP: case SDLK_z: case SDLK_w: return DIR_UP;
        case SDLK_DOWN: case SDLK_s:             return DIR_DOWN;
        case SDLK_LEFT: case SDLK_q: case SDLK_a:return DIR_LEFT;
        case SDLK_RIGHT: case SDLK_d:            return DIR_RIGHT;
        default: return -1;
    }
}

static int wait_game_over(SDL_Renderer *ren, int winner, GameState *gs) {
    render_draw(ren, gs, -1);
    render_game_over(ren, winner, gs);
    audio_play_sfx(SFX_GAMEOVER);

    SDL_Event ev;
    while (1) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) return 0;
            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_SPACE)  { audio_play_sfx(SFX_UI_CLICK); return 1; }
                if (ev.key.keysym.sym == SDLK_ESCAPE) { audio_play_sfx(SFX_UI_CLICK); return 0; }
            }
            if (ev.type == SDL_MOUSEBUTTONDOWN) {
                int mx=ev.button.x, my=ev.button.y;
                int ww=480, wh=340;
                int wx=WINDOW_W/2-ww/2, wy=WINDOW_H/2-wh/2;
                SDL_Rect btn_r={wx+ww/2-105,wy+wh-36,100,26};
                SDL_Rect btn_q={wx+ww/2+5,  wy+wh-36,100,26};
                if(mx>=btn_r.x&&mx<btn_r.x+btn_r.w&&my>=btn_r.y&&my<btn_r.y+btn_r.h)
                    { audio_play_sfx(SFX_UI_CLICK); return 1; }
                if(mx>=btn_q.x&&mx<btn_q.x+btn_q.w&&my>=btn_q.y&&my<btn_q.y+btn_q.h)
                    { audio_play_sfx(SFX_UI_CLICK); return 0; }
            }
        }
        SDL_Delay(16);
    }
}

// Ecran d'au revoir
static void show_farewell(SDL_Renderer *ren) {
    // Fond bureau
    SDL_SetRenderDrawColor(ren, 0, 128, 128, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0, 112, 112, 255);
    for(int y=0;y<WINDOW_H+44;y+=4)
        for(int x=0;x<WINDOW_W+8;x+=4)
            SDL_RenderDrawPoint(ren,x,y);

    // Fenetre Win98 centree
    int ww=320, wh=160;
    int wx=WINDOW_W/2-ww/2, wy=WINDOW_H/2-wh/2;

    // Fond fenetre biseaute
    SDL_SetRenderDrawColor(ren,192,192,192,255);
    SDL_Rect win={wx,wy,ww,wh}; SDL_RenderFillRect(ren,&win);
    // Barre de titre
    SDL_SetRenderDrawColor(ren,0,0,128,255);
    SDL_Rect tb={wx+3,wy+3,ww-6,18}; SDL_RenderFillRect(ren,&tb);
    render_string(ren,wx+7,wy+6,"ZONIX.EXE",(SDL_Color){255,255,255,255},1);

    // Message
    render_string(ren,wx+(ww-(int)strlen("AU REVOIR !")*16)/2,wy+35,
                  "AU REVOIR !",(SDL_Color){0,0,128,255},2);
    render_string(ren,wx+(ww-(int)strlen("MERCI D AVOIR JOUE")*8)/2,wy+65,
                  "MERCI D AVOIR JOUE",(SDL_Color){0,0,0,255},1);
    render_string(ren,wx+(ww-(int)strlen("ZONIX V4.0 - YNOV B1")*8)/2,wy+82,
                  "ZONIX V4.0 - YNOV B1",(SDL_Color){80,80,80,255},1);
    render_string(ren,wx+(ww-(int)strlen("MARLY & FLORENCE")*8)/2,wy+94,
                  "MARLY & FLORENCE",(SDL_Color){0,0,128,255},1);

    // Bouton OK
    SDL_Rect btn={wx+ww/2-40,wy+wh-36,80,24};
    SDL_SetRenderDrawColor(ren,192,192,192,255); SDL_RenderFillRect(ren,&btn);
    // bevel manuel
    SDL_SetRenderDrawColor(ren,255,255,255,255);
    SDL_RenderDrawLine(ren,btn.x,btn.y,btn.x+btn.w-1,btn.y);
    SDL_RenderDrawLine(ren,btn.x,btn.y,btn.x,btn.y+btn.h-1);
    SDL_SetRenderDrawColor(ren,80,80,80,255);
    SDL_RenderDrawLine(ren,btn.x+btn.w-1,btn.y,btn.x+btn.w-1,btn.y+btn.h-1);
    SDL_RenderDrawLine(ren,btn.x,btn.y+btn.h-1,btn.x+btn.w-1,btn.y+btn.h-1);
    render_string(ren,btn.x+(80-16)/2,btn.y+8,"OK",(SDL_Color){0,0,0,255},1);

    SDL_RenderPresent(ren);

    // Attend clic ou touche (max 3s)
    SDL_Event ev;
    Uint32 start = SDL_GetTicks();
    while(SDL_GetTicks()-start < 3000) {
        while(SDL_PollEvent(&ev)) {
            if(ev.type==SDL_KEYDOWN||ev.type==SDL_MOUSEBUTTONDOWN||ev.type==SDL_QUIT)
                return;
        }
        SDL_Delay(16);
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
    audio_init();

    NetworkConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    strncpy(cfg.ip, "127.0.0.1", sizeof(cfg.ip)-1);
    cfg.port = SERVER_PORT;

    LobbyInfo lobby;
    memset(&lobby, 0, sizeof(lobby));
    lobby.color_idx = 0;
    lobby.grid_size = 1;
    lobby.theme     = THEME_RETRO;

    int reconnect_same_server = 0;

    while (1) {
        int connected = 0, player_id = -1;
        disconnect_client();

        if (!reconnect_same_server) {
            while (!connected) {
                MenuResult res = menu_main(ren);
                if (res == MENU_RESULT_QUIT) goto quit;

                MenuResult net_res = menu_network(ren, &cfg, 0);
                if (net_res == MENU_RESULT_BACK)  continue;
                if (net_res == MENU_RESULT_QUIT)  goto quit;

                menu_connecting(ren, cfg.ip, cfg.port);
                if (init_client(cfg.ip, cfg.port) == 0) {
                    player_id = client_get_player_id();
                    connected = 1;
                } else {
                    SDL_SetRenderDrawColor(ren,0,128,128,255);
                    SDL_RenderClear(ren);
                    render_string(ren,WINDOW_W/2-90,WINDOW_H/2-10,"CONNEXION ECHOUEE",(SDL_Color){220,53,69,255},2);
                    render_string(ren,WINDOW_W/2-100,WINDOW_H/2+20,"VERIFIEZ L IP ET LE PORT",(SDL_Color){180,180,180,255},1);
                    render_string(ren,WINDOW_W/2-80,WINDOW_H/2+36,"APPUIE SUR UNE TOUCHE",(SDL_Color){128,128,128,255},1);
                    SDL_RenderPresent(ren);
                    SDL_Event ev; int w=1;
                    while(w){ while(SDL_PollEvent(&ev)) if(ev.type==SDL_KEYDOWN||ev.type==SDL_QUIT) w=0; SDL_Delay(16); }
                }
            }
        } else {
            reconnect_same_server = 0;
            menu_connecting(ren, cfg.ip, cfg.port);
            if (init_client(cfg.ip, cfg.port) == 0) {
                player_id = client_get_player_id();
                connected = 1;
            } else { continue; }
        }

        printf("[CLIENT] Joueur %d. Lobby...\n", player_id);

        if (lobby_screen(ren, &lobby, player_id) < 0) {
            disconnect_client(); continue;
        }

        LobbyPacket lp;
        lp.player_id = player_id;
        memcpy(lp.pseudo, lobby.pseudo, 16);
        lp.color_idx = lobby.color_idx;
        send_lobby(&lp);
        if (player_id == 1)
            send_config(lobby.grid_size, (int)lobby.theme);

        SDL_Delay(100);

        // Attente premier GameState
        GameState gs;
        memset(&gs, 0, sizeof(gs));
        {
            SDL_Event ev; int waiting=1, anim=0, attempts=0;
            while (waiting) {
                while (SDL_PollEvent(&ev)) {
                    if (ev.type==SDL_QUIT) goto quit;
                    if (ev.type==SDL_KEYDOWN&&ev.key.keysym.sym==SDLK_ESCAPE)
                        { disconnect_client(); goto next_round; }
                }
                if (anim%30==0) menu_waiting(ren,0,1);
                anim++;
                int r=receive_state(&gs);
                if (r>0&&gs.player_count>0) { waiting=0; }
                else if (r<0) { disconnect_client(); goto next_round; }
                if (++attempts>500) { disconnect_client(); goto next_round; }
                SDL_Delay(16);
            }
        }

        audio_play_sfx(SFX_START);
        printf("[CLIENT] Partie lancee !\n");

        // Boucle de jeu
        {
            SDL_Event ev;
            int running=1, game_over=0, winner=0;
            int prev_countdown = gs.countdown;
            Direction cur_dir=gs.players[player_id-1].dir;
            Uint32 last_send=SDL_GetTicks();
            int prev_alive=1;
            int prev_score=0;

            while (running) {
                while (SDL_PollEvent(&ev)) {
                    if (ev.type==SDL_QUIT) goto quit;
                    if (ev.type==SDL_KEYDOWN) {
                        if (ev.key.keysym.sym==SDLK_ESCAPE&&!game_over) running=0;
                        int dir=key_to_dir(ev.key.keysym.sym);
                        if (dir>=0&&!game_over) { cur_dir=(Direction)dir; audio_play_sfx(SFX_UI_CLICK); }
                    }
                }

                Uint32 now=SDL_GetTicks();
                if (now-last_send>=(Uint32)(TICK_RATE/2)) {
                    send_input(player_id,cur_dir);
                    last_send=now;
                }

                GameState tmp;
                int r=receive_state(&tmp);
                if (r>0) {
                    GameState old=gs;
                    gs=tmp;

                    // Sons contextuels
                    if(player_id>=1&&player_id<=gs.player_count){
                        Player *me=&gs.players[player_id-1];
                        Player *old_me=&old.players[player_id-1];

                        // Capture de territoire
                        if(me->score > prev_score+10)
                            audio_play_sfx(SFX_CAPTURE);
                        prev_score=me->score;

                        // Mort du joueur local
                        if(prev_alive&&!me->alive) {
                            audio_play_sfx(SFX_DEATH);
                            prev_alive=0;
                        }

                        // Kill ennemi
                        if(me->kills > old_me->kills)
                            audio_play_sfx(SFX_KILL);

                        // Power-up ramassé
                        if(me->captures > old_me->captures)
                            audio_play_sfx(SFX_POWERUP);
                    }

                    // Tick countdown
                    if(gs.countdown>0&&gs.countdown<prev_countdown)
                        audio_play_sfx(SFX_COUNTDOWN);
                    prev_countdown=gs.countdown;

                    if(!gs.running&&!game_over) {
                        game_over=1;
                        winner=game_winner(&gs);
                        running=0;
                    }
                } else if (r<0) {
                    running=0;
                }

                if(!game_over) render_draw(ren,&gs,player_id);
                SDL_Delay(8);
            }

            if (game_over) {
                EndStats stats;
                memset(&stats,0,sizeof(stats));
                game_build_stats(&gs,&stats);
                lobby_show_stats(ren,&stats,player_id,gs.theme);

                int replay=wait_game_over(ren,winner,&gs);
                if (replay) reconnect_same_server=1;
            }
        }

next_round:
        disconnect_client();
        continue;
    }

quit:
    audio_shutdown();
    disconnect_client();
    show_farewell(ren);
    render_cleanup(win,ren);
    return 0;
}