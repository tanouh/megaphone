#include "../constants.h"
#include "../msghead.h"
#include "../reqcode.h"
#include "action.h"
#include "cmesslib.h"
#include "view.h"
#include "notifications.h"
#include <pthread.h>
#include "mutex.h"
#include "../array.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>

static void show_notifications();

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
	address_sock.sin_port = htons(DEFAULT_SERVER_PORT+1);
	inet_pton(AF_INET, DEFAULT_SERVER_ADDR, &address_sock.sin_addr);

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
	struct msghead h = fill_msghead(INSCRIPTION, 0, 0, 0, 0);
	char buf[SIZE_MSG];
	int size = fill_inscription(h, buf, SIZE_MSG, buff_send,
				    strlen(buff_send));
	int envoye = send(socket, buf, size, 0);
	if (envoye < 0) {
		perror("Erreur d'envoi");
		return 1;
	}

	char buff_rcv[SIZE_MSG + 1];
	memset(&buff_rcv, 0, SIZE_MSG + 1);

	int recu = recv(socket, buff_rcv, SIZE_MSG, 0);
	if (recu < 0) {
		perror("Erreur de réception");
		return 1;
	}

	memset(&h, 0, sizeof(h));
	get_message(buff_rcv, &h);
	id = h.id;
	if (reqtoi(h.req) == ERROR) {
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
	switch (reqtoi(choose_action())) {
	case 0:
		show_notifications();
		break;
	case 1:
		// Poster un billet
		if (post(id, sock) == -1)
			print_error("Le billet n'a pas pu être poster.\n");
		break;
	case 2:
		// Ajouter fichier
		if (add_file() == -1)
			print_error("Erreur lors de l'ajout du fichier.\n");
		break;
	case 3:
		// Voir anciens billets
		if (see_old_ticket() == -1)
			print_error("Erreur lors de l'affichage des "
				    "précédents "
				    "fichiers.\n");

		break;
	case 4:
		// Télécharger fichier
		if (download() == -1)
			print_error("Erreur lors du téléchargement du "
				    "fichier.\n");
		break;
	case 5:
		// S'abonner à un fil
		if (subscribe() == -1)
			print_error("Erreur lors de l'abonnement au "
				    "fil.\n");
		break;
	default:
		print_error("Option invalide.\n");
	}
}

static void show_notifications() {
	if (count_notifications() == 0) {
		printf("Vous n'avez pas de nouvelles notifications");
		return;
	}
	pthread_mutex_lock(&mutex[M_NOTMESS]);
	struct array *q = copy_array(notmess, NULL);
	clear(notmess, NULL);
	pthread_mutex_unlock(&mutex[M_NOTMESS]);
	print_notifications(q);
	free_array(q, NULL);
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
