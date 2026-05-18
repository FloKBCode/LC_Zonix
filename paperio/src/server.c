// server.c - Florence : creation du socket serveur TCP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <pthread.h>
#include "../include/server.h"
#include "../include/threads.h"
#include "../include/constants.h"
#include "../include/types.h"

static SOCKET server_sock;
SOCKET client_sockets[MAX_PLAYERS];
int client_count = 0;
GameState game_state;

int init_server(int port) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Erreur WSAStartup\n");
        return -1;
    }
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("Erreur creation socket\n");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Erreur bind\n");
        return -1;
    }
    if (listen(server_sock, MAX_PLAYERS) == SOCKET_ERROR) {
        printf("Erreur listen\n");
        return -1;
    }
    printf("Serveur en ecoute sur le port %d\n", port);
    return 0;
}

void run_server() {
    game_state.running = 1;

    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, NULL, thread_accept,    (void*)&server_sock);
    pthread_create(&t2, NULL, thread_receive,   NULL);
    pthread_create(&t3, NULL, thread_game_loop, NULL);
    pthread_create(&t4, NULL, thread_send,      NULL);

    // attend que tous les threads se terminent
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
}

void close_server() {
    for (int i = 0; i < client_count; i++)
        closesocket(client_sockets[i]);
    closesocket(server_sock);
    WSACleanup();
    printf("Serveur ferme\n");
}
