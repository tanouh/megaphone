#include "../constants.h"
#include "../msghead.h"
#include "../reqcode.h"
#include "cmesslib.h"
#include "view.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int post(void *id, void *arg)
{
	struct msghead h;
	memset(&h, 0, sizeof(h));
	int sock_client = *(int *)arg;
	h.id = *(int *)id;

	/*On récupère le numéro de la fil de discu*/
	h.chat = choose_thread_to_post_in();

	/*Le contenu du billet*/
	char *text = post_ticket(h.chat);
	printf("%s\n", text);

	h.datalen = (uint8_t)strlen(text);
	h.req = PUSH_MESS;
	char buf[SBUF];
	memset(buf, 0, SBUF);
	/*On remplit l'en-tête pour avoir le format de message à envoyer au
	 * serveur*/
	int size = fill_message(h, buf, SBUF, text);
	free(text);

	/*Envoi et réception*/
	if (send(sock_client, buf, size, 0) <= 0) {
		perror("Sending message to the server failed.\n");
		return -1;
	}
	memset(buf, 0, SBUF);

	if (recv(sock_client, buf, SBUF, 0) <= 0) {
		perror("Receiving message from the server failed.\n");
		return -1;
	}
	printf("%s\n", buf);
	return 0;
}

int add_file()
{
	char *file_link = add_file_to_thread();
	printf("%s\n", file_link);

	// TODO : Ajouter fichier
	return 0;
}

int see_old_ticket()
{
	int nchat = select_thread_to_unfold();
	if (nchat == 0) {
		print_error("Numéro de thread incorrect.\n");
		return -1;
	}
	int nb_ticket = see_previous_tickets(nchat);
	if (nb_ticket == 0) {
		print_error("Nombre de billets à voir invalide.\n");
		return -1;
	}
	printf("%d\n", nb_ticket);

	// TODO : Voir anciens billets

	return 0;
}

int download()
{
	int num_file = download_file();
	if (num_file == 0) {
		print_error("Numéro du fichier à télécharger invalide.\n");
		return -1;
	}
	printf("%d\n", num_file);

	// TODO : Télécharger fichiers

	return 0;
}

int subscribe()
{
	int num_thread = subscribe_to_thread();
	if (num_thread == 0) {
		print_error("Numéro du fil auquel s'abonner.\n");
		return -1;
	}
	printf("%d\n", num_thread);

	// TODO : S'abonner à un fil

	return 0;
}
