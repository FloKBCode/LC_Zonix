// server.c - serveur TCP avec restart automatique
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <pthread.h>
#include "../include/server.h"
#include "../include/threads.h"
#include "../include/constants.h"
#include "../include/types.h"

static SOCKET server_sock = INVALID_SOCKET;

SOCKET    client_sockets[MAX_PLAYERS];
int       client_count = 0;
GameState game_state;

// FIX #5 : init_server ne prend plus nb_players (expected_players/humans
// sont déjà set par main_server.c, on ne les écrase plus)
int init_server(int port) {
    for (int i = 0; i < MAX_PLAYERS; i++)
        client_sockets[i] = INVALID_SOCKET;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return -1;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) return -1;

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("[SERVER] Erreur bind port %d\n", port);
        return -1;
    }
    if (listen(server_sock, MAX_PLAYERS) == SOCKET_ERROR) return -1;

    printf("[SERVER] En ecoute sur le port %d (%d humains + %d bots)\n",
           port, expected_humans, expected_players - expected_humans);
    return 0;
}

void run_server(void) {
    while (1) {
        client_count = 0;
        game_started = 0;
        for (int i = 0; i < MAX_PLAYERS; i++)
            client_sockets[i] = INVALID_SOCKET;
        memset(&game_state, 0, sizeof(game_state));
        game_state.running = 0;

        printf("[SERVER] En attente de %d humain(s)...\n", expected_humans);

        pthread_t t1, t2, t3, t4;
        pthread_create(&t1, NULL, thread_accept,    (void*)&server_sock);
        pthread_create(&t2, NULL, thread_receive,   NULL);
        pthread_create(&t3, NULL, thread_game_loop, NULL);
        pthread_create(&t4, NULL, thread_send,      NULL);

        pthread_join(t3, NULL);
        game_state.running = 0;

        Sleep(2000);
        printf("[SERVER] Partie terminee. Redemarrage dans 3s...\n");
        Sleep(3000);
        printf("[SERVER] Nouvelle partie !\n");
    }
}

void close_server(void) {
    for (int i = 0; i < MAX_PLAYERS; i++)
        if (client_sockets[i] != INVALID_SOCKET)
            closesocket(client_sockets[i]);
    if (server_sock != INVALID_SOCKET)
        closesocket(server_sock);
    WSACleanup();
}

void server_broadcast(void) {
    for (int i = 0; i < expected_humans; i++) {
        if (client_sockets[i] == INVALID_SOCKET) continue;
        send(client_sockets[i], (char*)&game_state, sizeof(GameState), 0);
    }
}
