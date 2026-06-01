// client.c - connexion TCP au serveur (Florence)
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "../include/client.h"
#include "../include/types.h"

static SOCKET sock     = INVALID_SOCKET;
static int    my_id    = -1;

int init_client(const char *ip, int port) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("[CLIENT] Erreur WSAStartup\n");
        return -1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("[CLIENT] Erreur creation socket\n");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (addr.sin_addr.s_addr == INADDR_NONE) {
        printf("[CLIENT] Adresse IP invalide : %s\n", ip);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("[CLIENT] Connexion impossible a %s:%d\n", ip, port);
        return -1;
    }

    printf("[CLIENT] Connecte a %s:%d\n", ip, port);

    // Recoit l'ID de joueur assigne par le serveur
    int received_id = -1;
    int n = recv(sock, (char *)&received_id, sizeof(int), 0);
    if (n == sizeof(int) && received_id >= 1 && received_id <= MAX_PLAYERS) {
        my_id = received_id;
        printf("[CLIENT] ID de joueur recu : %d\n", my_id);
    } else {
        printf("[CLIENT] Erreur reception ID joueur\n");
        return -1;
    }

    return 0;
}

void send_input(int player_id, Direction dir) {
    if (sock == INVALID_SOCKET) return;
    Packet pkt;
    pkt.player_id = player_id;
    pkt.dir       = dir;
    send(sock, (char *)&pkt, sizeof(Packet), 0);
}

int receive_state(GameState *state_buf) {
    if (sock == INVALID_SOCKET) return -1;

    // Lecture complete du GameState (peut arriver en plusieurs segments TCP)
    char *buf = (char *)state_buf;
    int total  = 0;
    int needed = (int)sizeof(GameState);

    while (total < needed) {
        int n = recv(sock, buf + total, needed - total, 0);
        if (n <= 0) return -1;
        total += n;
    }
    return total;
}

void disconnect_client(void) {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    WSACleanup();
    printf("[CLIENT] Deconnecte\n");
}

int client_get_player_id(void) {
    return my_id;
}
