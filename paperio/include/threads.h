// threads.h - les 4 threads du serveur
#ifndef THREADS_H
#define THREADS_H

#include <winsock2.h>

void *thread_accept(void *arg);     // accepte les nouvelles connexions
void *thread_receive(void *arg);    // lit les inputs des clients
void *thread_game_loop(void *arg);  // logique du jeu (tick a TICK_RATE ms)
void *thread_send(void *arg);       // broadcast l'etat a tous les clients

// Drapeau global : le jeu a commence (tous les joueurs sont connectes)
extern volatile int game_started;

// Nombre de joueurs attendus (configure par le serveur au demarrage)
extern int expected_players;

#endif
