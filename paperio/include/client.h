// client.h - Florence : connexion TCP cote client
#ifndef CLIENT_H
#define CLIENT_H

int  init_client(const char *ip, int port);
void send_input(int player_id, int dir);
int  receive_state(void *state_buf);
void disconnect_client();

#endif
