// main_client.c - point d'entree du client
#include <stdio.h>
#include <winsock2.h>
#include "../include/client.h"
#include "../include/constants.h"

int main() {
    // ouvre une console pour voir les messages
    AllocConsole();
    freopen("CONOUT\$", "w", stdout);
    freopen("CONIN\$", "r", stdin);

    char ip[64];
    printf("IP du serveur : ");
    fgets(ip, sizeof(ip), stdin);
    // enleve le \n a la fin
    ip[strcspn(ip, "\n")] = 0;

    if (init_client(ip, SERVER_PORT) < 0) {
        printf("Connexion impossible\n");
        Sleep(2000);
        return 1;
    }
    printf("Connecte ! Appuie sur Entree pour quitter\n");
    getchar();
    disconnect_client();
    return 0;
}
