// server.h - init et gestion du serveur TCP
#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include "types.h"
#include "constants.h"

// Variables accessibles depuis les threads
extern SOCKET     client_sockets[MAX_PLAYERS];
extern int        client_count;
extern GameState  game_state;

int  init_server(int port, int expected_players);
void run_server(void);
void close_server(void);

// Broadcast l'etat du jeu a tous les clients connectes
void server_broadcast(void);

#endif
