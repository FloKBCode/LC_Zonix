// main_client.c - point d'entree du client
#include <stdio.h>
#include "../include/client.h"
#include "../include/constants.h"

int main() {
    char ip[64];
    printf("IP du serveur : ");
    scanf("%s", ip);
    if (init_client(ip, SERVER_PORT) < 0) {
        printf("Connexion impossible\n");
        return 1;
    }
    printf("Appuie sur Entree pour quitter\n");
    getchar(); getchar();
    disconnect_client();
    return 0;
}
