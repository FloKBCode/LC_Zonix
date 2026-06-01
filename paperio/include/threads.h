#ifndef THREADS_H
#define THREADS_H

#include <winsock2.h>
#include <pthread.h>

void *thread_accept(void *arg);
void *thread_receive(void *arg);
void *thread_game_loop(void *arg);
void *thread_send(void *arg);

extern volatile int    game_started;
extern int             expected_players;
extern int             expected_humans;
extern pthread_mutex_t gs_mutex;

#endif
