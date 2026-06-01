// server.c - creation et gestion du serveur TCP (Florence)
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

int init_server(int port, int nb_players) {
    expected_players = nb_players;

    // Initialise tous les sockets clients a invalide
    for (int i = 0; i < MAX_PLAYERS; i++)
        client_sockets[i] = INVALID_SOCKET;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("[SERVER] Erreur WSAStartup\n");
        return -1;
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("[SERVER] Erreur creation socket\n");
        return -1;
    }

    // Option SO_REUSEADDR pour relancer rapidement sans attendre TIME_WAIT
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("[SERVER] Erreur bind sur le port %d\n", port);
        return -1;
    }

    if (listen(server_sock, MAX_PLAYERS) == SOCKET_ERROR) {
        printf("[SERVER] Erreur listen\n");
        return -1;
    }

    printf("[SERVER] En ecoute sur le port %d (attente de %d joueurs)\n",
           port, nb_players);
    return 0;
}

void run_server(void) {
    game_state.running = 0;  // sera mis a 1 par game_init dans thread_accept

    pthread_t t1, t2, t3, t4;

    pthread_create(&t1, NULL, thread_accept,    (void *)&server_sock);
    pthread_create(&t2, NULL, thread_receive,   NULL);
    pthread_create(&t3, NULL, thread_game_loop, NULL);
    pthread_create(&t4, NULL, thread_send,      NULL);

    // Le serveur attend la fin du game_loop (t3)
    pthread_join(t3, NULL);

    // Signale l'arret aux autres threads
    game_state.running = 0;

    // Attend les autres threads avec un timeout (2s)
    Sleep(2000);

    printf("[SERVER] Serveur arrete\n");
}

void close_server(void) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (client_sockets[i] != INVALID_SOCKET)
            closesocket(client_sockets[i]);
    }
    if (server_sock != INVALID_SOCKET)
        closesocket(server_sock);
    WSACleanup();
    printf("[SERVER] Ressources liberees\n");
}

void server_broadcast(void) {
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == INVALID_SOCKET) continue;
        send(client_sockets[i], (char *)&game_state, sizeof(GameState), 0);
    }
}
