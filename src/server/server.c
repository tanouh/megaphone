#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


int main (int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "run with : ./server <PORT>\n");
		exit(1);
	}

	/* Création de l'adresse destinataire (serveur) */
	struct sockaddr_in6 address_sock;
	address_sock.sin6_family = AF_INET6;
	address_sock.sin6_port = htons(atoi(argv[1]));
	address_sock.sin6_addr = in6addr_any;

	/*  ########## AFFICHAGE INTRO ########### */

	/* Création de la socket */
	int sock = socket(PF_INET6, SOCK_STREAM, 0);
	if (sock < 0){
		perror("fail in socket creation");
		exit(1);
	}

	/* Désactivation de l'option " n'accepter que de l'IPV6 " */
	int optval = 0;
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval)) < 0)
		perror("connexion with IPV4 impossible");

	optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		perror("port reuse impossible");
	
	/* on lie la socket au port */
	if (bind(sock, (struct sockaddr *) &address_sock, sizeof(address_sock)) < 0) {
		perror("binding failed");
		exit(2);
	}

	/* le serveur est prêt à écouter les connexions sur le port ... */

	if (listen(sock, 0) < 0){
		perror("listen error");
		exit(2);
	}

	/* la boucle du serveur */

	while (1) {
	
	/* le serveur accepte une connexion 
	et crée la socket de communication avec le client */

		struct sockaddr_in6 adrclient;
		memset(&adrclient, 0, sizeof(adrclient));
		socklen_t size = sizeof(adrclient);

		/* on crée la variable sur le tas */
		int *sockclient = malloc(sizeof(int));
		if (sockclient == NULL){
			perror("malloc failed");
			exit(2); /* ou break ? */
		}

		*sockclient = accept(sock, (struct sockaddr *) &adrclient, &size);

		if (sockclient >= 0){
			pthread_t thread;

			/* le serveur crée un thread 
			* a priori il va falloir faire un traitement des messages du client ici 
			*/


		}
	}
	close(sock);
	return 0;
} 