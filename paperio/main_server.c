// main_server.c - point d'entree du serveur (console pure, sans SDL2)
#include <stdio.h>
#include <string.h>
#include "../include/server.h"
#include "../include/threads.h"
#include "../include/constants.h"

int main(void) {
    int port    = SERVER_PORT;
    int nb      = 2;
    char buf[8];

    printf("=== ZONIX - Serveur ===\n");
    printf("Nombre de joueurs (2-4) [defaut: 2] : ");
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin) && buf[0] != '\n')
        nb = atoi(buf);
    if (nb < 2) nb = 2;
    if (nb > 4) nb = 4;

    printf("Port [defaut: %d] : ", SERVER_PORT);
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin) && buf[0] != '\n') {
        int p = atoi(buf);
        if (p > 0) port = p;
    }

    printf("Lancement du serveur pour %d joueurs sur le port %d...\n", nb, port);

    if (init_server(port, nb) < 0) {
        printf("Impossible de demarrer le serveur\n");
        return 1;
    }

    run_server();
    close_server();

    printf("Partie terminee. Appuie sur Entree pour quitter.\n");
    getchar();
    return 0;
}
