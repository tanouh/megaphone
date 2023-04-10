#include "printlib.h"
#include "test_constants.h"

#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mserv = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;

void print_serv(char *msg)
{
	pthread_mutex_lock(&mserv);
	printf("%s: %s", SERVER, msg);
	pthread_mutex_unlock(&mserv);
}

void print_client(char *msg)
{
	pthread_mutex_lock(&mserv);
	printf("%s: %s", CLIENT, msg);
	pthread_mutex_unlock(&mserv);
}

void print_meg(char *msg)
{
	pthread_mutex_lock(&mp);
	printf("%s: %s\n", MEGAPHONE, msg);
	pthread_mutex_unlock(&mp);
}

