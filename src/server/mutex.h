#ifndef MEGAPHONE_MUTEX_H
#define MEGAPHONE_MUTEX_H

#include <pthread.h>

#define M_NB 4
#define M_ALL_CHATS 0
#define M_CHAT_COUNTER 1
#define M_IDS 2
#define M_CLIENT_COUNTER 3
extern pthread_mutex_t mutex[];

void init_mutex();

#endif
