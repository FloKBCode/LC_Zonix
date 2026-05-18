// server.c - Florence : creation du socket serveur TCP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "../include/server.h"
#include "../include/constants.h"
#include "../include/types.h"

// socket global du serveur
static SOCKET server_sock;

// liste des clients connectes
SOCKET client_sockets[MAX_PLAYERS];
int client_count = 0;

// etat du jeu partage entre tous les threads
GameState game_state;

int init_server(int port) {
    // initialisation de winsock (obligatoire sur Windows)
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Erreur WSAStartup\n");
        return -1;
    }

    // creation du socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("Erreur creation socket\n");
        return -1;
    }

    // configuration de l'adresse
    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // bind : attache le socket au port
    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Erreur bind\n");
        return -1;
    }

    // listen : attend les connexions (max MAX_PLAYERS en attente)
    if (listen(server_sock, MAX_PLAYERS) == SOCKET_ERROR) {
        printf("Erreur listen\n");
        return -1;
    }

    printf("Serveur en ecoute sur le port %d\n", port);
    return 0;
}

void close_server() {
    for (int i = 0; i < client_count; i++)
        closesocket(client_sockets[i]);
    closesocket(server_sock);
    WSACleanup();
    printf("Serveur ferme\n");
}

void run_server() {
    // TODO : demarrer les 4 threads
    printf("run_server() appelé - threads a implementer\n");
}
