#include <pthread.h>
#include "printlib.h"
#include "test_constants.h"
#include <stdio.h>

pthread_mutex_t mserv = PTHREAD_MUTEX_INITIALIZER;

void print_serv(char *msg){
	pthread_mutex_lock(&mserv);
	printf("%s: %s", SERVER, msg);
	pthread_mutex_unlock(&mserv);
}

void print_client(char *msg, int id){
	pthread_mutex_lock(&mserv);
	printf("\x1B[36m[CLIENT %d]\x1B[0m: %s", id, msg);
	pthread_mutex_unlock(&mserv);
}
