// threads.h - Florence : les 4 threads du serveur
#ifndef THREADS_H
#define THREADS_H

void *thread_accept(void *arg);
void *thread_receive(void *arg);
void *thread_game_loop(void *arg);
void *thread_send(void *arg);

#endif
