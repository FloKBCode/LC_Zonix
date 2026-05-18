// server.h - Florence : init et gestion du serveur TCP
#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include "types.h"
#include "constants.h"

// variables accessibles depuis les threads
extern SOCKET client_sockets[MAX_PLAYERS];
extern int client_count;
extern GameState game_state;

int  init_server(int port);
void run_server();
void close_server();

#endif
