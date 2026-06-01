#ifndef CLIENT_H
#define CLIENT_H

#include "types.h"

int  init_client(const char *ip, int port);
void send_lobby(LobbyPacket *lp);
void send_config(int grid_size, int theme);
void send_input(int player_id, Direction dir);
int  receive_state(GameState *state_buf);
void disconnect_client(void);
int  client_get_player_id(void);

#endif
