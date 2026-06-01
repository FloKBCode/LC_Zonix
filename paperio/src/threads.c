// threads.c - les 4 threads du serveur (Florence)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "../include/threads.h"
#include "../include/server.h"
#include "../include/game.h"
#include "../include/types.h"
#include "../include/constants.h"

volatile int game_started = 0;
int expected_players      = 2;  // sera mis a jour par main_server

// =============================================================================
// Thread 1 : accept - attend de nouvelles connexions
// =============================================================================
void *thread_accept(void *arg) {
    SOCKET srv_sock = *(SOCKET *)arg;
    printf("[ACCEPT] Thread demarre, attente de %d joueurs...\n", expected_players);

    while (client_count < expected_players) {
        SOCKET new_sock = accept(srv_sock, NULL, NULL);
        if (new_sock == INVALID_SOCKET) {
            printf("[ACCEPT] Erreur accept\n");
            continue;
        }

        int id = client_count;  // 0-indexed
        client_sockets[id] = new_sock;
        printf("[ACCEPT] Joueur %d connecte\n", id + 1);

        // Envoie l'ID de joueur au client (simple int)
        int player_id = id + 1;
        send(new_sock, (char *)&player_id, sizeof(int), 0);

        client_count++;
    }

    printf("[ACCEPT] Tous les joueurs sont connectes. Demarrage du jeu.\n");

    // Initialise et lance le jeu
    game_init(&game_state, expected_players);
    game_started = 1;

    return NULL;
}

// =============================================================================
// Thread 2 : receive - lit les inputs de chaque client
// =============================================================================
void *thread_receive(void *arg) {
    (void)arg;
    printf("[RECEIVE] Thread demarre\n");

    while (game_state.running || !game_started) {
        if (!game_started) {
            Sleep(10);
            continue;
        }

        for (int i = 0; i < client_count; i++) {
            if (client_sockets[i] == INVALID_SOCKET) continue;

            Packet pkt;
            // recv non-bloquant : on verifie juste s'il y a un paquet
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(client_sockets[i], &fds);
            struct timeval tv = { 0, 1000 };  // timeout 1ms
            int ready = select(0, &fds, NULL, NULL, &tv);

            if (ready > 0) {
                int n = recv(client_sockets[i], (char *)&pkt, sizeof(Packet), 0);
                if (n == sizeof(Packet)) {
                    // Validation de l'ID
                    if (pkt.player_id >= 1 && pkt.player_id <= MAX_PLAYERS) {
                        // Anti-demi-tour : on ne peut pas faire demi-tour
                        Direction old = game_state.players[pkt.player_id - 1].dir;
                        Direction nd  = pkt.dir;
                        int ok = 1;
                        if ((old == DIR_UP    && nd == DIR_DOWN)  ||
                            (old == DIR_DOWN  && nd == DIR_UP)    ||
                            (old == DIR_LEFT  && nd == DIR_RIGHT) ||
                            (old == DIR_RIGHT && nd == DIR_LEFT))
                            ok = 0;
                        if (ok)
                            game_state.players[pkt.player_id - 1].dir = nd;
                    }
                } else if (n == 0) {
                    // Client deconnecte
                    printf("[RECEIVE] Joueur %d deconnecte\n", i + 1);
                    if (game_state.players[i].alive)
                        game_state.players[i].alive = 0;
                    closesocket(client_sockets[i]);
                    client_sockets[i] = INVALID_SOCKET;
                }
            }
        }
    }
    return NULL;
}

// =============================================================================
// Thread 3 : game_loop - avance le jeu a TICK_RATE ms
// =============================================================================
void *thread_game_loop(void *arg) {
    (void)arg;
    printf("[GAME] Thread demarre\n");

    while (1) {
        if (!game_started) {
            Sleep(10);
            continue;
        }

        if (!game_state.running) {
            Sleep(500);  // laisse le thread send envoyer l'etat final
            break;
        }

        game_update(&game_state);
        Sleep(TICK_RATE);
    }

    printf("[GAME] Partie terminee. Gagnant : Joueur %d\n",
           game_winner(&game_state));
    return NULL;
}

// =============================================================================
// Thread 4 : send - diffuse l'etat du jeu a tous les clients
// =============================================================================
void *thread_send(void *arg) {
    (void)arg;
    printf("[SEND] Thread demarre\n");

    while (1) {
        if (!game_started) {
            // Envoie quand meme le nb de connectes pour l'ecran d'attente
            Sleep(200);
            continue;
        }

        for (int i = 0; i < client_count; i++) {
            if (client_sockets[i] == INVALID_SOCKET) continue;
            int n = send(client_sockets[i],
                         (char *)&game_state, sizeof(GameState), 0);
            if (n <= 0) {
                closesocket(client_sockets[i]);
                client_sockets[i] = INVALID_SOCKET;
            }
        }

        if (!game_state.running) break;
        Sleep(TICK_RATE);
    }

    printf("[SEND] Thread termine\n");
    return NULL;
}
