#include "view.h"
#include "cmesslib.h"
#include "../reqcode.h"
#include "../constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int post(void * id, void *arg)
{
	int sock_client = *(int *)arg;
	uint16_t id_client = *(int *)id;
	
	/*On récupère le numéro de la fil de discu*/
	int num_thread = choose_thread_to_post_in();
	
	/*Le contenu du billet*/
	char *text = post_ticket(num_thread);
	printf("%s\n", text);
	
	int size_msg = 0; 
	uint8_t datalen = (uint8_t)strlen(text);

	/*On remplit l'en-tête pour avoir le format de message à envoyer au serveur*/
	void *msg = fill_push_message(id_client,num_thread, datalen, text, &size_msg);
	free(text);

	/*Envoi et réception*/
	if(send(sock_client, msg, strlen((char *)msg), 0) <= 0){
		perror("Sending message to the server failed.\n");
		return -1;
	}
	free(msg);
	char buf[SBUF];
	memset(buf,0,sizeof(buf));

	if(recv(sock_client,buf,SBUF * sizeof(char), 0 ) <= 0){
		perror("Receiving message from the server failed.\n");
		return -1;
	}
	printf("%s\n",buf);
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
	int num_thread = select_thread_to_unfold();
	if (num_thread == 0) {
		print_error("Numéro de thread incorrect.\n");
		return 1;
	}
	int nb_ticket = see_previous_tickets(num_thread);
	if (nb_ticket == 0) {
		print_error("Nombre de billets à voir invalide.\n");
		return 1;
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
		return 1;
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
		return 1;
	}
	printf("%d\n", num_thread);

	// TODO : S'abonner à un fil

	return 0;
}