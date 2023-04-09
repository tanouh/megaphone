#include "../constants.h"
#include "../msghead.h"
#include "../reqcode.h"
#include "cmesslib.h"
#include "view.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int registering(void *id, void *arg)
{
	char *buff_send = ask_pseudo();
	struct msghead h = fill_msghead(INSCRIPTION, 0, 0, 0, 0);
	char buf[SIZE_MSG];
	int socket = *(int *)arg;

	int size = fill_inscription(h, buf, SIZE_MSG, buff_send,
				    strlen(buff_send));
	int envoye = send(socket, buf, size, 0);
	if (envoye < 0) {
		perror("Erreur d'envoi");
		return -1;
	}
	
	char buff_rcv[SIZE_MSG + 1];
	memset(&buff_rcv, 0, SIZE_MSG + 1);

	int recu = recv(socket, buff_rcv, SIZE_MSG, 0);
	if (recu < 0) {
		perror("Erreur de réception");
		return -1;
	}

	memset(&h, 0, sizeof(h));
	if (get_message(buff_rcv, &h) == -1){
		print_error("Echec de formatage");
		return -1;
	}
	*(int *)id = h.id;
	if (reqtoi(h.req) == ERROR) {
		print_error("Erreur d'inscription");
	}
	if (id == 0)
		print_error("Pseudo trop court");
	else
		show_id(*(int *)id); 
	return 0;
}
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

int see_old_ticket(void *id, void *arg)
{
	struct msghead h;
	memset(&h, 0, sizeof(h));
	int sock_client = *(int *)arg;
	h.id = *(int *)id; 

	h.chat = select_thread_to_unfold();
	int nb_ticket = see_previous_tickets(h.chat);
	printf("%d\n", nb_ticket);

	h.datalen = 0;
	h.req = ASK_MESS;
	char buf[SBUF];
	memset(buf, 0, SBUF);

	int size = fill_message(h, buf, SBUF, "");

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
	printf("%s\n", buf); // y mettre plus d'esthétique

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
