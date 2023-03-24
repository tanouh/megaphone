#include "../constants.h"
#include "../lib.h"
#include "../map.h"
#include "slib.h"
#include "smesslib.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define NBCLIENTS 10
#define SIZE_MSG 256

int server;

int get_server_port(int argc, char *argv[])
{
	if (argc != 2)
		return PORT;
	else
		return atoi(argv[1]);
}

int create_server(int port)
{
	struct sockaddr_in6 address_sock;
	address_sock.sin6_family = AF_INET6;
	address_sock.sin6_port = htons(port);
	address_sock.sin6_addr = in6addr_any;

	server = socket(PF_INET6, SOCK_STREAM, 0);
	if (server < 0) {
		perror("fail in socket creation");
		return -1;
	}
	int optval = 0;
	if (setsockopt(server, IPPROTO_IPV6, IPV6_V6ONLY, &optval,
		       sizeof(optval)) < 0)
		perror("connexion with IPV4 impossible");

	optval = 1;
	if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval,
		       sizeof(optval)) < 0)
		perror("port reuse impossible");
	if (bind(server, (struct sockaddr *)&address_sock,
		 sizeof(address_sock)) < 0) {
		perror("binding failed");
		return -1;
	}
	if (listen(server, 0) < 0) {
		perror("listen error");
		return -1;
	}
	return 0;
}

int accept_inscription(void *sockclient)
{
	char buff_rcv[SIZE_MSG];
	memset(&buff_rcv, 0, SIZE_MSG + 1);

	int recu = recv(*(int *)sockclient, buff_rcv, SIZE_MSG, 0);
	if (recu < 0) {
		perror("Erreur de reception");
		return -1;
	}

	enum reqcode *req;
	uint16_t id;
	char *nickname;
	get_inscription(buff_rcv, req, &id, &nickname);
	if (reqtoi(*req) != 1) {
		perror("Erreur de requete");
		return -1;
	}

	id = new_id();

	if (!put_map(identifiers, &id, &nickname, NULL, sizeof(uint16_t),
		     sizeof(char *))) {
		perror("Erreur de stockage de l'identifiant");
		return -1;
	}

	return 0;
}

void *init(void *sockclient)
{
	print_s("Connexion établie\n");
	accept_inscription(sockclient);
	// TODO : création de thread qui exécute les actions ?
	return NULL;
}

int connect_to_client(int *sockclient, pthread_t *thread)
{

	/* le serveur accepte une connexion
	et crée la socket de communication avec le client */

	struct sockaddr_in6 adrclient;
	memset(&adrclient, 0, sizeof(adrclient));
	socklen_t size = sizeof(adrclient);
	*sockclient = accept(server, (struct sockaddr *)&adrclient, &size);

	if (*sockclient < 0) {
		return -1;
	}
	if (pthread_create(thread, NULL, init, sockclient) == -1) {
		perror("Thread creation failed");
		return -1;
	}
	return 0;
}

int serve(int port)
{

	if (create_server(port) == -1)
		return 1;
	print_s("Ouverture du serveur \nEN ATTENTE DE CONNEXION...\n");

	pthread_t threads[NBCLIENTS];
	int *scs = calloc(NBCLIENTS, sizeof(int));
	if (scs == NULL) {
		perror("malloc failed");
		close(server);
		return 1;
	}
	int c_connected = 0;
	while (c_connected < NBCLIENTS) {
		if (connect_to_client(scs + c_connected,
				      &threads[c_connected]) == -1)
			break;
		c_connected++;
	}

	next_id = 1;
	identifiers = make_map(compare_identifiers, default_hash);

	int ret = 1;
	int *tmp;
	for (int i = 0; i < c_connected; i++) {
		if (pthread_join(threads[i], (void **)&tmp) != 0)
			perror("thread join");
		if (tmp != NULL) {
			ret |= *tmp;
			free(tmp);
		} else {
			ret = 0;
		}
	}
	free(scs);
	char buf[SBUF];
	sprintf(buf, "Fin de session (%d)\n", !ret);
	free_map(identifiers, free_identifier, free_nickname);
	print_s(buf);
	close(server);
	return !ret;
}

int main(int argc, char *argv[])
{
	int port = get_server_port(argc, argv);
	return serve(port);
}
