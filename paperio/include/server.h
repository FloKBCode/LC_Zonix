#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include "types.h"
#include "constants.h"

extern SOCKET     client_sockets[MAX_PLAYERS];
extern int        client_count;
extern GameState  game_state;

int  init_server(int port);   // FIX: ne prend plus nb_players
void run_server(void);
void close_server(void);
void server_broadcast(void);

#endif
