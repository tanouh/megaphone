#include "../constants.h"
#include "../msghead.h"
#include "../reqcode.h"
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
void take_action()
{
	switch (choose_action()) {
	case INSCRIPTION:
		if(registering(&id, &sock) == -1){
			print_error("L'inscription a échoué.\n");
		}
		break;
	case PUSH_MESS:
		// Poster un billet
		if (post(&id, &sock) == -1)
			print_error("Le billet n'a pas pu être poster.\n");
		break;
	case PUSH_FILE:
		// Ajouter fichier
		if (add_file() == -1)
			print_error("Erreur lors de l'ajout du fichier.\n");
		break;
	case ASK_MESS:
		// Voir anciens billets
		if (see_old_ticket(&id, &sock) == -1)
			print_error("Erreur lors de l'affichage des "
				    "précédents "
				    "fichiers.\n");

		break;
	case PULL_FILE:
		// Télécharger fichier
		if (download() == -1)
			print_error("Erreur lors du téléchargement du "
				    "fichier.\n");
		break;
	case SUBSCRIBE:
		// S'abonner à un fil
		if (subscribe() == -1)
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
	take_action(); 
	//todo : demander id dans les autres actions que inscription
	close(sock);
	return 0;
}

int main()
{
	return client();
}
