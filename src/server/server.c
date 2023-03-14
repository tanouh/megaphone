#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define DEFAULT_PORT 2424
#define NBCLIENTS 10

int get_server_port (int argc, char *argv[]){
	if (argc != 2) 
		return DEFAULT_PORT;
	else
		return atoi(argv[1]);	
}

int create_server(int port){
	struct sockaddr_in6 address_sock;
	address_sock.sin6_family = AF_INET6;
	address_sock.sin6_port = htons(port);
	address_sock.sin6_addr = in6addr_any;

	/*  ########## AFFICHAGE INTRO ########### */

	int sock = socket(PF_INET6, SOCK_STREAM, 0);
	if (sock < 0){
		perror("fail in socket creation");
		exit(1);
	}
	int optval = 0;
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval)) < 0)
		perror("connexion with IPV4 impossible");

	optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		perror("port reuse impossible");
	if (bind(sock, (struct sockaddr *) &address_sock, sizeof(address_sock)) < 0) {
		perror("binding failed");
		exit(2);
	}
	if (listen(sock, 0) < 0){
		perror("listen error");
		exit(2);
	}
	return sock;
}

int connect_to_client (int sock) {

	/* le serveur accepte une connexion
	et crée la socket de communication avec le client */

	struct sockaddr_in6 adrclient;
	memset(&adrclient, 0, sizeof(adrclient));
	socklen_t size = sizeof(adrclient);

	int *sockclient = malloc(sizeof(int));
	if (sockclient == NULL){
		perror("malloc failed");
		return -1; 
	}
	*sockclient = accept(sock, (struct sockaddr *) &adrclient, &size);

	if (sockclient >= 0){
		pthread_t thread;
		/* 
		* TODO : le serveur crée un thread 
		* a priori il va falloir faire un traitement des messages du client ici 
		*/
	}
	return 0;
}

int main (int argc, char *argv[]) {
	int port = get_server_port(argc, argv);
	int server = create_server(port);

	/*
	* TODO: Créer un tableau de thread qui stocke les threads
	*/
	pthread_t threads[NBCLIENTS];

	while (1) {
		if (connect_to_client(server) == -1) break;
		/*
		* attendre que tous les threads se terminent 
		*/
	}
	close(server);
	return 0;
}
