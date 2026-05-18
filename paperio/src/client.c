// client.c - Florence : connexion TCP au serveur
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "../include/client.h"
#include "../include/types.h"

static SOCKET sock;

int init_client(const char *ip, int port) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Erreur WSAStartup\n");
        return -1;
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Erreur creation socket\n");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Erreur connexion au serveur %s:%d\n", ip, port);
        return -1;
    }
    printf("Connecte au serveur %s:%d\n", ip, port);
    return 0;
}

void send_input(int player_id, int dir) {
    Packet pkt;
    pkt.player_id = player_id;
    pkt.dir       = dir;
    send(sock, (char*)&pkt, sizeof(Packet), 0);
}

int receive_state(void *state_buf) {
    return recv(sock, (char*)state_buf, sizeof(GameState), 0);
}

void disconnect_client() {
    closesocket(sock);
    WSACleanup();
    printf("Deconnecte du serveur\n");
}
