// client.c - connexion TCP + envoi lobby/config
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "../include/client.h"
#include "../include/types.h"

static SOCKET sock  = INVALID_SOCKET;
static int    my_id = -1;

int init_client(const char *ip, int port) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return -1;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return -1;

    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    if (addr.sin_addr.s_addr == INADDR_NONE) return -1;
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) return -1;

    printf("[CLIENT] Connecte a %s:%d\n", ip, port);

    // Recoit l'ID assigne par le serveur (bloquant, court)
    int received_id = -1;
    int n = recv(sock, (char*)&received_id, sizeof(int), 0);
    if (n == sizeof(int) && received_id >= 1 && received_id <= MAX_PLAYERS) {
        my_id = received_id;
        printf("[CLIENT] ID joueur : %d\n", my_id);
        return 0;
    }
    return -1;
}

void send_lobby(LobbyPacket *lp) {
    if (sock == INVALID_SOCKET) return;
    send(sock, (char*)lp, sizeof(LobbyPacket), 0);
    printf("[CLIENT] LobbyPacket envoye (pseudo=%s color=%d)\n",
           lp->pseudo, lp->color_idx);
}

void send_config(int grid_size, int theme) {
    if (sock == INVALID_SOCKET) return;
    int conf[2] = { grid_size, theme };
    send(sock, (char*)conf, sizeof(conf), 0);
    printf("[CLIENT] Config envoyee (grid=%d theme=%d)\n", grid_size, theme);
}

void send_input(int player_id, Direction dir) {
    if (sock == INVALID_SOCKET) return;
    Packet pkt = { player_id, dir };
    send(sock, (char*)&pkt, sizeof(Packet), 0);
}

// Retourne >0 si un GameState complet a ete recu
// Retourne 0 si rien disponible (non-bloquant)
// Retourne -1 si connexion perdue
int receive_state(GameState *state_buf) {
    if (sock == INVALID_SOCKET) return -1;

    // Verifie si des donnees sont disponibles (5ms timeout)
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    struct timeval tv = {0, 5000};
    int ready = select(0, &fds, NULL, NULL, &tv);
    if (ready <= 0) return 0;  // rien disponible

    // Lecture complete du GameState (TCP peut fragmenter)
    char *buf = (char*)state_buf;
    int total  = 0;
    int needed = (int)sizeof(GameState);

    while (total < needed) {
        // Verifie si la suite est dispo avant de bloquer
        FD_ZERO(&fds);
        FD_SET(sock, &fds);
        struct timeval tv2 = {0, 100000}; // 100ms max par fragment
        int r2 = select(0, &fds, NULL, NULL, &tv2);
        if (r2 <= 0) return 0; // fragment pas encore arrive, on reessaiera

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

int client_get_player_id(void) { return my_id; }