#include "mutex.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex[M_NB];

void init_mutex() {
	pthread_mutex_t init = PTHREAD_MUTEX_INITIALIZER;
	for (size_t i = 0; i < M_NB; i++) {
		memcpy(mutex + i, &init, sizeof(init));
	}
}
