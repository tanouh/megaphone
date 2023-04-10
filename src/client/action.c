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
	if (h.req == ERROR) {
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
	h.chat = choose_chat_to_post_in();

	char *text = post_ticket(h.chat);
	printf("%s\n", text);

	h.datalen = (uint8_t)strlen(text);
	h.req = PUSH_MESS;

	char buf[SBUF+1];
	memset(buf, 0, SBUF+1);
	int size = fill_message(h, buf, SBUF+1, text);
	free(text);
	
	int r = send(sock_client, buf, size, 0);
	if (r <= 0) {
		perror("Sending message to the server failed.\n");
		return -1;
	}
	memset(buf, 0, SBUF+1);
	r = recv(sock_client, buf, SBUF+1, 0);
	if (r <= 0) {
		perror("Receiving message from the server failed.\n");
		return -1;
	}
	memcpy(buf+r, "/0", 1);
	printf("%s\n", buf);
	return 0;
}

int add_file()
{
	char *file_link = add_file_to_chat();
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

	h.chat = select_chat_to_unfold();
	int nb_ticket = see_previous_tickets(h.chat);
	printf("%d\n", nb_ticket);

	h.datalen = 0;
	h.req = ASK_MESS;
	char buf[SBUF+1];
	memset(buf, 0, SBUF+1);

	int size = fill_message(h, buf, SBUF+1, "");

	int r = send(sock_client, buf, size, 0);
	if (r <= 0) {
		perror("Sending message to the server failed.\n");
		return -1;
	}
	memset(buf, 0, SBUF+1);
	r = recv(sock_client, buf, SBUF+1, 0);
	if (r <= 0) {
		perror("Receiving message from the server failed.\n");
		return -1;
	}
	memcpy(buf+r, "/0", 1);
	printf("%s\n", buf);
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
	int nchat= subscribe_to_chat();
	if (nchat == 0) {
		print_error("Numéro du fil auquel s'abonner.\n");
		return -1;
	}
	printf("%d\n", nchat);

	// TODO : S'abonner à un fil

	return 0;
}
