#include "action.h"
#include "cmesslib.h"
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
#define ERROR 31

uint16_t id;
int sock;

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

static int registering(int socket)
{
	char *buff_send = ask_pseudo();
	int size_msg;
	void *buff = fill_inscription(buff_send, strlen(buff_send), &size_msg);

	int envoye = send(socket, buff, size_msg, 0);
	free(buff);
	if (envoye < 0) {
		perror("Erreur d'envoi");
		return 1;
	}

	char buff_rcv[SIZE_MSG];
	memset(&buff_rcv, 0, SIZE_MSG + 1);

	int recu = recv(socket, buff_rcv, SIZE_MSG, 0);
	if (recu < 0) {
		perror("Erreur de réception");
		return 1;
	}

	enum reqcode req=0;
	uint16_t id;

	get_message(buff_rcv, &req, &id, NULL, NULL);
	if (reqtoi(req) == ERROR) {
		print_error("Erreur d'inscription");
	}
	if (id == 0) {
		print_error("Pseudo trop court");
	} else
		show_id(id);
	return 0;
}

void take_action()
{
	switch (choose_action()) {
	case 0:
		print_error("Réponse invalide.\n");
		break;
	case 1:
		// Poster un billet
		if (post(id, sock))
			print_error("Le billet n'a pas pu être poster.\n");
		break;
	case 2:
		// Ajouter fichier
		if (add_file())
			print_error("Erreur lors de l'ajout du fichier.\n");
		break;
	case 3:
		// Voir anciens billets
		if (see_old_ticket())
			print_error("Erreur lors de l'affichage des "
				    "précédents "
				    "fichiers.\n");

		break;
	case 4:
		// Télécharger fichier
		if (download())
			print_error("Erreur lors du téléchargement du "
				    "fichier.\n");
		break;
	case 5:
		// S'abonner à un fil
		if (subscribe())
			print_error("Erreur lors de l'abonnement au "
				    "fil.\n");
		break;
	default:
		print_error("Option invalide.\n");
	}
}

static int client()
{
	if (connect_to_server() == 1)
		return 1;
	if (registering(sock))
		return 1;
	while (1) {
		take_action();
	}
	close(sock);

	return 0;
}

int main()
{
	return client();
}
