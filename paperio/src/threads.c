// threads.c - ANTI-SPOOF player_id : identité = socket, pas le paquet
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <pthread.h>
#include "../include/threads.h"
#include "../include/server.h"
#include "../include/game.h"
#include "../include/bot.h"
#include "../include/types.h"
#include "../include/constants.h"

volatile int game_started = 0;
int expected_players      = 2;
int expected_humans       = 1;
pthread_mutex_t gs_mutex  = PTHREAD_MUTEX_INITIALIZER;

static int is_uturn(Direction old, Direction nd) {
    return (old==DIR_UP&&nd==DIR_DOWN)||(old==DIR_DOWN&&nd==DIR_UP)||
           (old==DIR_LEFT&&nd==DIR_RIGHT)||(old==DIR_RIGHT&&nd==DIR_LEFT);
}

void *thread_accept(void *arg) {
    SOCKET srv_sock = *(SOCKET*)arg;
    printf("[ACCEPT] Attente de %d humain(s)...\n", expected_humans);

    while (client_count < expected_humans) {
        SOCKET new_sock = accept(srv_sock, NULL, NULL);
        if (new_sock == INVALID_SOCKET) continue;

        int id = client_count;
        client_sockets[id] = new_sock;
        printf("[ACCEPT] Joueur %d connecte\n", id+1);

        int player_id = id+1;
        send(new_sock, (char*)&player_id, sizeof(int), 0);

        LobbyPacket lp; memset(&lp, 0, sizeof(lp));
        int n = recv(new_sock, (char*)&lp, sizeof(LobbyPacket), 0);
        if (n == sizeof(LobbyPacket)) {
            if (lp.color_idx < 0 || lp.color_idx >= COLOR_PALETTE_SIZE)
                lp.color_idx = id % COLOR_PALETTE_SIZE;
            memcpy(game_state.players[id].pseudo, lp.pseudo, 15);
            game_state.players[id].pseudo[15] = '\0'; // AUDIT: force NUL
            game_state.players[id].color_idx = lp.color_idx;
            printf("[ACCEPT] Pseudo : %s\n", game_state.players[id].pseudo);
        } else {
            snprintf(game_state.players[id].pseudo, 16, "P%d", id+1);
            game_state.players[id].color_idx = id % COLOR_PALETTE_SIZE;
        }
        client_count++;
    }

    int grid_conf=1, theme_conf=0;
    if (expected_humans >= 1 && client_sockets[0] != INVALID_SOCKET) {
        fd_set fds; FD_ZERO(&fds); FD_SET(client_sockets[0], &fds);
        struct timeval tv = {1, 0};
        if (select(0, &fds, NULL, NULL, &tv) > 0) {
            int conf[2];
            int n = recv(client_sockets[0], (char*)conf, sizeof(conf), 0);
            if (n == sizeof(conf)) { grid_conf=conf[0]; theme_conf=conf[1]; }
        }
    }

    int gw, gh;
    switch (grid_conf) {
        case 0: gw=60; gh=45; break;
        case 2: gw=100; gh=75; break;
        default: gw=GRID_W; gh=GRID_H; break;
    }

    pthread_mutex_lock(&gs_mutex);
    game_state.theme = (Theme)theme_conf;
    game_init(&game_state, expected_players, gw, gh);

    for (int i = expected_humans; i < expected_players; i++) {
        bot_init(i+1);
        if (game_state.players[i].pseudo[0] == '\0')
            snprintf(game_state.players[i].pseudo, 16, "BOT%d", i-expected_humans+1);
        game_state.players[i].color_idx = (expected_humans+i) % COLOR_PALETTE_SIZE;
        printf("[BOT] Bot %d en (%d,%d)\n", i+1,
               game_state.players[i].x, game_state.players[i].y);
    }
    pthread_mutex_unlock(&gs_mutex);

    game_started = 1;
    printf("[ACCEPT] Jeu lance ! %d humain(s) + %d bot(s)\n",
           expected_humans, expected_players-expected_humans);
    return NULL;
}

void *thread_receive(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&gs_mutex);
        int run = game_state.running;
        pthread_mutex_unlock(&gs_mutex);
        if (!run && game_started) break;
        if (!game_started) { Sleep(10); continue; }

        pthread_mutex_lock(&gs_mutex);

        // Humains — AUDIT: identité = index socket i, PAS pkt.player_id
        for (int i = 0; i < expected_humans; i++) {
            if (client_sockets[i] == INVALID_SOCKET) continue;
            fd_set fds; FD_ZERO(&fds); FD_SET(client_sockets[i], &fds);
            struct timeval tv = {0, 500};
            if (select(0, &fds, NULL, NULL, &tv) > 0) {
                Packet pkt;
                int n = recv(client_sockets[i], (char*)&pkt, sizeof(Packet), 0);
                if (n == sizeof(Packet)) {
                    // Anti-spoof : on ignore pkt.player_id, on utilise i
                    Direction old = game_state.players[i].dir;
                    Direction nd  = pkt.dir;
                    if (!is_uturn(old, nd))
                        game_state.players[i].dir = nd;
                } else if (n == 0) {
                    printf("[RECEIVE] Joueur %d deconnecte\n", i+1);
                    game_state.players[i].alive = 0;
                    closesocket(client_sockets[i]);
                    client_sockets[i] = INVALID_SOCKET;
                }
            }
        }

        // Bots
        for (int i = expected_humans; i < expected_players; i++) {
            if (!game_state.players[i].alive) continue;
            game_state.players[i].dir = bot_think(&game_state, i+1);
        }

        pthread_mutex_unlock(&gs_mutex);
        Sleep(5);
    }
    return NULL;
}

void *thread_game_loop(void *arg) {
    (void)arg;
    printf("[GAME] Thread demarre\n");
    while (1) {
        if (!game_started) { Sleep(10); continue; }
        if (!game_state.running) { Sleep(500); break; }
        pthread_mutex_lock(&gs_mutex);
        game_update(&game_state);
        pthread_mutex_unlock(&gs_mutex);
        Sleep(TICK_RATE);
    }
    printf("[GAME] Fin. Gagnant : Joueur %d\n", game_winner(&game_state));
    return NULL;
}

void *thread_send(void *arg) {
    (void)arg;
    while (1) {
        if (!game_started) { Sleep(200); continue; }
        pthread_mutex_lock(&gs_mutex);
        for (int i = 0; i < expected_humans; i++) {
            if (client_sockets[i] == INVALID_SOCKET) continue;
            int n = send(client_sockets[i], (char*)&game_state, sizeof(GameState), 0);
            if (n <= 0) { closesocket(client_sockets[i]); client_sockets[i]=INVALID_SOCKET; }
        }
        pthread_mutex_unlock(&gs_mutex);
        if (!game_state.running) {
            Sleep(500); server_broadcast();
            Sleep(500); server_broadcast();
            break;
        }
        Sleep(TICK_RATE);
    }
    return NULL;
}