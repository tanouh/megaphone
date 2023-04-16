#include "view.h"

#include "../lib.h"
#include "notifications.h"
#include "../array.h"

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

#define C_RED	"\x1B[31m"
#define C_GREEN "\x1B[32m"
#define C_CLEAR "\x1B[0m"
#define C_YELLOW "\x1B[33m"

static void print_req(char *m, enum reqcode r);
static void print_notification_message(struct notification *n);

char *ask_pseudo()
{
	printf("Bienvenue sur le serveur *Nom Provisoire*\nVeuillez saisir un "
		"pseudo de 10 caractères\n");

	return readline("Pseudo : ");
}

void welcome(int already_registered)
{
	if (already_registered)
		printf("%sConnexion réussie !%s\n", C_GREEN, C_CLEAR);
	else
		printf("%sInscription réussie !%s\n", C_GREEN, C_CLEAR);
}

void show_id(uint16_t id)
{
	printf("Le serveur Megaphone vous a attribué l'identifiant %u.\n", id);
}

enum reqcode choose_action()
{
	printf("Que souhaitez-vous faire ?\nPour :\n");
	print_req("\t- poster un billet, tapez %d\n", PUSH_MESS);
	print_req("\t- ajouter un fichier, tapez %d\n", PUSH_FILE);
	print_req("\t- voir des billets ""précedents, tapez %d\n", ASK_MESS);
	print_req("\t- télécharger un fichier, tapez %d\n", PULL_FILE);
	print_req("\t- vous abonner à un fil, tapez %d\n", SUBSCRIBE);
	size_t cn = count_notifications();
	if (cn > -1) {
		printf("Lire les notifications [%lu nouvelles], tapez %d\n", cn, reqtoi(_NOTIFICATION));
	}
	char *answer = readline("Action : ");
	int ans = atoi(answer);
	if (ans == 0 && strcmp(answer, "0"))
		return -1;
	return itoreq(ans);
}

int choose_thread_to_post_in()
{
	printf("Vous avez choisi de poster un billet.\nDans quel fil "
	       "souhaitez-vous le poster ?\n");
	return atoi(readline("Numéro du fil : "));
}

char *post_ticket(int num_thread)
{
	printf("Vous avez choisi le fil %d.\n", num_thread);
	return readline("Contenu du billet à poster : ");
}

char *add_file_to_thread()
{
	print_c("Vous avez choisi d'ajouter un fichier.\n");
	return readline("Lien du fichier I guess : ");
}

int select_thread_to_unfold()
{
	print_c("Vous avez choisi de voir des billets précédents.\n");
	return atoi(readline("Numéro du fil à dérouler : "));
}

int see_previous_tickets(int num_thread)
{
	printf("Combien de tickets de %d voulez-vous afficher ?\n", num_thread);
	return atoi(readline("Nombre de tickets à afficher : "));
}

int download_file()
{
	print_c("Vous avez choisi de télécharger un fichier.\n");
	return atoi(readline("Numéro du fichier I guess :"));
}

int subscribe_to_thread()
{
	print_c("Vous avez choisi de vous abonner à un fil.\n");
	return atoi(readline("Numéro du fil I guess :"));
}

void print_error(char *error)
{
	printf("%s%s%s\n", C_RED, error, C_CLEAR);
}

void print_notification(struct array *q) {
	for (int i = 0; i < q->size; i++) {
		print_notification_message(at(q, i));
	}
}

static void print_req(char *m, enum reqcode r)
{
	printf(m, reqtoi(r));
}

static void print_notification_message(struct notification *n) {
	printf("%sFil %u\n%s", C_YELLOW, n->chat, C_CLEAR);
	printf("Auteur : %s\n", n->author);
	printf("\n%s\n\n", n->content);
}

