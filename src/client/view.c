#include "view.h"

#include "../lib.h"

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

#define C_RED "\x1B[31m"
#define C_GREEN "\x1B[32m"
#define C_CLEAR "\x1B[0m"

char *ask_pseudo()
{
	print_c("Bienvenue sur le serveur *Nom Provisoire*\nVeuillez saisir un "
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

int choose_action()
{
	print_c("Que souhaitez-vous faire ?\nPour :\n - poster un billet, "
		"tapez "
		"1\n - ajouter un fichier, taper 2\n - voir des billets "
		"précedents, taper 3\n - télécharger un fichier, taper 4\n - "
		"vous abonner à un fil, taper 5\n");
	return atoi(readline("Action : "));
}

int choose_thread_to_post_in()
{
	print_c("Vous avez choisi de poster un billet.\nDans quel fil "
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