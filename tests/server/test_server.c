/*
This is the server side test file.
It will test server functions and the communication with the client.
*/

#include "../../src/lib.h"
#include "../../src/server/smesslib.h"
#include "../printlib.h"
#include "../test_constants.h"
#include "../testlib.h"
#include "test_smesslib.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define NBTEST 1
#define NBCLIENT 1

int id = 1;
#define SIZE_MESS 512

pthread_mutex_t p = PTHREAD_MUTEX_INITIALIZER;

void print(char *msg)
{
	pthread_mutex_lock(&p);
	print_serv(msg);
	pthread_mutex_unlock(&p);
}

void print_connected_client(struct sockaddr_in adrclient)
{
	char buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(adrclient.sin_addr), buf, INET_ADDRSTRLEN);
	char msg[SBUF];
	sprintf(msg, "client %d connected (%s %d)\n", id, buf,
		adrclient.sin_port);
	print_serv(msg);
	id++;
}

int create_server()
{
	print_serv("Creating server...\n");
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ntohs(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int yes = 1;
	int r = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (r < 0)
		perror("setsockopt");
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
		return -1;
	if (listen(sock, 0) != 0)
		return -1;
	return sock;
}

void *test_network()
{
	int server = create_server();
	kill(getppid(), SIGUSR1);
	if (server < 0)
		return malloc_return(ESYS);
	pthread_t pts[NBCLIENT];
	int *scs = calloc(NBCLIENT, sizeof(int));
	int cpt = 0;

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	while (cpt < NBCLIENT) {
		scs[cpt] = accept(server, (struct sockaddr *)&addr, &addrlen);
		if (scs[cpt] < 0) {
			perror("accept");
			return NULL;
		}
		print_connected_client(addr);
		if (pthread_create(pts + cpt, NULL, test_smesslib, scs + cpt) !=
		    0) {
			perror("thread create");
			return NULL;
		}
		cpt++;
	}
	int ret = 1;
	for (int i = 0; i < NBCLIENT; i++) {
		int *tmp;
		if (pthread_join(pts[i], (void *)&tmp) != 0)
			perror("thread join");
		if (tmp != NULL) {
			ret |= *tmp;
			free(tmp);
		} else {
			ret = 1;
		}
	}
	return (ret) ? malloc_return(ret) : NULL;
}

int main()
{
	pthread_t pts[NBTEST];
	if (pthread_create(pts, NULL, (void *(*)(void *))test_network, NULL) ==
	    -1) {
		perror("Thread didn't laucnhed");
		return ESYS;
	}

	int ret = 1;
	print_serv("Waiting for thread to finish...\n");
	for (int i = 0; i < NBTEST; i++) {
		int *tmp;
		if (pthread_join(pts[i], (void *)&tmp) != 0)
			perror("thread join");
		if (tmp != NULL) {
			ret &= *tmp;
			free(tmp);
		} else {
			ret = 0;
		}
	}
	char buf[512];
	sprintf(buf, "End of server (%d)\n", !ret);
	print_serv(buf);
	return !ret;
}
