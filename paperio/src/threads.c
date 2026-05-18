// threads.c - Florence : implementation des 4 threads
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "../include/threads.h"
#include "../include/server.h"
#include "../include/types.h"
#include "../include/constants.h"

// thread 1 : attend les nouvelles connexions
void *thread_accept(void *arg) {
    (void)arg;
    printf("Thread accept demarre\n");
    while (client_count < MAX_PLAYERS) {
        SOCKET new_client = accept(*(SOCKET*)arg, NULL, NULL);
        if (new_client == INVALID_SOCKET) continue;
        client_sockets[client_count] = new_client;
        printf("Joueur %d connecte\n", client_count + 1);
        client_count++;
    }
    return NULL;
}

// thread 2 : lit les inputs envoyes par chaque client
void *thread_receive(void *arg) {
    (void)arg;
    printf("Thread receive demarre\n");
    while (game_state.running) {
        for (int i = 0; i < client_count; i++) {
            Packet pkt;
            int n = recv(client_sockets[i], (char*)&pkt, sizeof(Packet), 0);
            if (n <= 0) continue; // client deconnecte
            // met a jour la direction du joueur
            game_state.players[pkt.player_id].dir = pkt.dir;
        }
    }
    return NULL;
}

// thread 3 : logique du jeu, tourne a TICK_RATE ms
void *thread_game_loop(void *arg) {
    (void)arg;
    printf("Thread game_loop demarre\n");
    while (game_state.running) {
        // TODO : appeler update_game(&game_state)
        Sleep(TICK_RATE); // pause entre chaque tick (Windows)
    }
    return NULL;
}

// thread 4 : envoie l'etat du jeu a tous les clients
void *thread_send(void *arg) {
    (void)arg;
    printf("Thread send demarre\n");
    while (game_state.running) {
        for (int i = 0; i < client_count; i++) {
            send(client_sockets[i], (char*)&game_state, sizeof(GameState), 0);
        }
        Sleep(TICK_RATE);
    }
    return NULL;
}
