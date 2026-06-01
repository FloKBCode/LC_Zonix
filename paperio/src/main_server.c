// main_server.c - serveur console avec option bots
#include <stdio.h>
#include <stdlib.h>
#include "../include/server.h"
#include "../include/threads.h"
#include "../include/constants.h"

int main(void) {
    int port   = SERVER_PORT;
    int total  = 2;
    int humans = 1;
    char buf[8];

    printf("=== ZONIX - Serveur ===\n");

    printf("Joueurs humains (1-%d) [defaut: 1] : ", MAX_PLAYERS);
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin) && buf[0] != '\n')
        humans = atoi(buf);
    if (humans < 1) humans = 1;
    if (humans > MAX_PLAYERS) humans = MAX_PLAYERS;

    printf("Joueurs total humains+bots (>=%d, max %d) [defaut: 2] : ",
           humans, MAX_PLAYERS);
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin) && buf[0] != '\n')
        total = atoi(buf);
    if (total < humans) total = humans;
    if (total > MAX_PLAYERS) total = MAX_PLAYERS;

    printf("Port [defaut: %d] : ", SERVER_PORT);
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin) && buf[0] != '\n') {
        int p = atoi(buf);
        if (p > 0) port = p;
    }

    // FIX #5 : set AVANT init_server, qui ne les écrase plus
    expected_players = total;
    expected_humans  = humans;

    printf("Lancement : %d humain(s) + %d bot(s) sur le port %d\n",
           humans, total - humans, port);

    if (init_server(port) < 0) {
        printf("Impossible de demarrer le serveur\n");
        return 1;
    }

    run_server();
    close_server();

    printf("Partie terminee. Appuie sur Entree.\n");
    getchar();
    return 0;
}
