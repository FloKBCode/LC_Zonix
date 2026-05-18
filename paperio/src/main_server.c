// main_server.c - Florence : point d'entree du serveur
#include <stdio.h>
#include "../include/server.h"
#include "../include/constants.h"

int main() {
    if (init_server(SERVER_PORT) < 0) {
        printf("Impossible de demarrer le serveur\n");
        return 1;
    }
    run_server();
    close_server();
    return 0;
}
