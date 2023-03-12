#include "view.h"

#include <arpa/inet.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>

#define SERVER_PORT 4242
#define SERVER_ADDR "fdc7:9dd5:2c66:be86:4849:43ff:fe49:79be/64"
#define SIZE_MSG 256

uint16_t id;
int sock;

static int registering(int socket)
{
	char *buff_send = ask_pseudo();

	int envoye = send(socket, buff_send, strlen(buff_send), 0);
	free(buff_send);
	if (envoye < 0) {
		perror("Erreur d'envoi");
		return 1;
	}

	char buff_rcv[SIZE_MSG];
	memset(&buff_rcv, 0, SIZE_MSG + 1);

	int recu = recv(socket, buff_rcv, SIZE_MSG * sizeof(char), 0);
	if (recu < 0) {
		perror("Erreur de réception");
		return 1;
	}

	// get_message(buff_rcv, enum reqcode *req, &id, NULL, NULL);
	// if (req == ERROR){
	// 	print_error("Erreur d'inscription");
	// }
	// show_id(id);

	return 0;
}

static int connect_to_server()
{
	/* ****Création de la socket ****   */
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Création de socket");
		return 1;
	}

	/* ****Création de l'adresse destinataire (serveur) **** */
	struct sockaddr_in address_sock;
	memset(&address_sock, 0, sizeof(address_sock));
	address_sock.sin_family = AF_INET;
	address_sock.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &address_sock.sin_addr);

	/* ****demande de connexion au serveur **** */
	int r = connect(sock, (struct sockaddr *)&address_sock,
			sizeof(address_sock));

	if (r == -1) {
		perror("Echec de connexion");
		return 1;
	}


	return 0;
}

static int client(){
	if (connect_to_server() == 1)
		return 1;
	if (registering(sock))
		return 1;
	close(sock);

	return 0;
}

int main()
{
	return client();
}