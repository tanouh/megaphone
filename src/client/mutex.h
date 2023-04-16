#ifndef MEGAPHONE_MUTEX_H
#define MEGAPHONE_MUTEX_H

#include <pthread.h>

#define M_NB	  4
#define M_NOTFDS  0
#define M_NOTMESS 1
extern pthread_mutex_t mutex[];

void init_mutex();

#endif
