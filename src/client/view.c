#include "view.h"

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

#define C_RED "\x1B[31m"
#define C_GREEN "\x1B[32m"
#define C_CLEAR "\x1B[0m"

char *ask_pseudo()
{
	printf("Bienvenue sur le serveur *Nom Provisoire*\n Veuillez saisir un "
	       "pseudo de 10 caractères\n");

	return readline("Pseudo : ");
}

void show_id(uint16_t id)
{
	printf("Le serveur Megaphone vous a attribué l'identifiant %u.", id);
}

void print_error(char *error)
{
	printf("%s%s%s\n", C_RED, error, C_CLEAR);
}