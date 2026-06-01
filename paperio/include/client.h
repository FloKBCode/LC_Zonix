// client.h - connexion TCP cote client
#ifndef CLIENT_H
#define CLIENT_H

#include "types.h"

int  init_client(const char *ip, int port);
void send_input(int player_id, Direction dir);
int  receive_state(GameState *state_buf);
void disconnect_client(void);

// Renvoie l'ID de joueur assigne par le serveur (1-4), ou -1 si erreur
int  client_get_player_id(void);

#endif
